/*
 * Simulation.cpp
 *
 *  Created on: 22 Dec 2010
 *      Author: b0wter
 */

#include "Simulation.h"
#include <sys/time.h>

namespace SimulationSpace
{

Simulation::Simulation(int argc, char** argv)
{
	// initialize members
	entityManager_ = new EntityManager();
	simulationOptions_ = new SimulationOptions();

	// the loop is used to quit the constructor prematurly (in case of an error)
	// constructors cannot be left with a simple 'return' so it is embedded in a once running
	// loop that can be quit by 'continue' or 'break'.
	do
	{

		// read config file (or try to do so)
		if(!this->readCommandLineArguments(argc, argv))
		{
			/*
			 * If there was an error reading the command line the specified
			 * config file was probably wrong. There is nothing the program
			 * can do about it, so lets hope it gets shut down properly.
			 */
			ERROR("There was an error in 'readCommandLineArguments(...), maybe you've forgotten the necessary parameters?")
				break;
		}

		// check all parameters and data
		if(!checkConfigurationAndData())
		{
			// in case of an error the output should have been printed in the checkConfigurationAndData function
			cerr << "checkConfig - Fehler!" << endl;
			break;
		}

		// enable the history
		vector<Particle*>::iterator iP;
		if(simulationOptions_->saveParticleHistory)
			for(iP = entityManager_->getParticles()->begin(); iP != entityManager_->getParticles()->end(); ++iP)
				(*iP)->enableHistory(simulationOptions_->timeStepNo);
		vector<Cloud*>::iterator iC;
		if(simulationOptions_->saveCloudHistory)
			for(iC = entityManager_->getClouds()->begin(); iC != entityManager_->getClouds()->end(); ++iC)
				(*iC)->enableHistory(simulationOptions_->timeStepNo);


		// print the complete simulation data in the terminal
		entityManager_->cloudsToTerminal(true);
		simulationOptions_->toTerminal();

		// initialize the interactions with the simulation options
		globalInteractions_ = new Interactions::OneBodyInteraction(simulationOptions_);
		twoBodyInteractions_ = new Interactions::TwoBodyInteractions(simulationOptions_);
		specialInteractions_ = new Interactions::SpecialInteractions(simulationOptions_, entityManager_);

		// print the system's total energy
		double totalEnergyStart = calculateTotalEnergy();
		cerr << setprecision(12)<< left << setw(30) << "Total energy at beginning: " << totalEnergyStart << endl;

		// Output statistics once to include the starting positions in the data file
		this->outputStatisticsHeader();
		this->outputStatistics();

		//cerr << simulationOptions_->harmonicInteractionMatrix;

		// set the function pointer
		if(simulationOptions_->equationSolver == equationSolverTypeEuler)
			move = &Simulation::moveEuler;
		else if(simulationOptions_->equationSolver == equationSolverTypeVelocityVerlet)
			move = &Simulation::moveVV;
		else if(simulationOptions_->equationSolver == equationSolverTypeUnknown)
		{
			move = NULL;
			ERROR("Unknown equation solver!")
			break;
		}

		// start the fun part
		double dt = simulationOptions_->timeStepSize;
		unsigned int steps = simulationOptions_->timeStepNo;

		// Perform an Euler step first to fill the variables with proper values. (currentForce, lastForce,...)
		this->moveEuler(dt,0);
		this->outputStatistics();
		this->checkBoundaryConditions();

		// Perform the rest of the calculation using some other algorithm
		for(unsigned int i = 1; i < steps; i++)
		{
			(this->*move)(dt, i);
			this->checkBoundaryConditions();
			this->outputStatistics();
		}

		// print the system's energy at the end
		double totalEnergyEnd = calculateTotalEnergy();
		cerr << setprecision(12) << left << setw(30) << "Total energy at end: " << totalEnergyEnd << endl;
		cerr << setprecision(12) << left << setw(30) << "Difference is: "<< setprecision(2) << totalEnergyEnd - totalEnergyStart << " (" << (totalEnergyStart-totalEnergyEnd) / totalEnergyStart * 100 << " %)" << endl;

	}
	while(false);
}

/*
 * Make sure that everyting that has to be freed is freed in here
 */
Simulation::~Simulation()
{
	delete simulationOptions_;
	delete entityManager_;
	delete globalInteractions_;
	delete twoBodyInteractions_;
}

/*
 * Calculates the forces acting on the particles
 */
INLINE void Simulation::calculateForces(vector<Particle*>* particles)
{
	unsigned int particlesNo = particles->size();

	for(unsigned int i = 0; i < particlesNo; i++)
	{
		// calculate the 'global' forces, like gravity
		globalInteractions_->compute((*particles)[i], i);

		for(unsigned int j = i + 1; j < particlesNo; j++)
			twoBodyInteractions_->compute(i, (*particles)[i], j, (*particles)[j]);
	}

	// compute the special interactions, which are seperated from the rest
	//specialInteractions_->compute();

	//HACK: Ausgabe der Kräfte
//	for(unsigned int i = 0; i < particlesNo; i++)
//	{
//		cerr << (*particles)[i]->getCurrentForce()->x() << "," << (*particles)[i]->getCurrentForce()->y() << endl;
//	}
//	cerr << "\n\n";
}

void Simulation::moveVV(double dt, unsigned int step)
{
	ENTER

	// create some local variables to store some data
	vector<Particle*>* localParticles = entityManager_->getParticles();
	unsigned int particlesNo = localParticles->size();

	// Calculate new positions
	for(unsigned int i = 0; i < particlesNo; i++)
	{
		Particle* p = (*localParticles)[i];

		p->position_ += dt * (p->velocity_ + (0.5 / p->mass_* p->currentForce_ * dt));

		// make the current force the last force and
		p->switchForce();
		// reset the current force for the next calculation
		p->resetCurrentForce();
	}

	// Calculate new forces
	calculateForces(localParticles);

	// Calculate the new velocities
	for(unsigned int i = 0; i < particlesNo; i++)
	{
		Particle* p = (*localParticles)[i];
		p->velocity_ += dt * (0.5 / p->mass_ * (p->currentForce_ + p->lastForce_));
	}

	LEAVE
}


// The old version of move(), when the move()-function was also implemented in the particles
// class. But that was difficult to handle with more complex algorithms that need to calculate
// the new position, then the new force and afterwards the new velocities
void Simulation::moveEuler(double dt, unsigned int step)
{
	ENTER	;

	unsigned int particlesNo = entityManager_->getParticles()->size();
	vector<Particle*>* localParticles = entityManager_->getParticles(); // save a reference to the particle list to save time

	calculateForces(entityManager_->getParticles());

	// iterate over all particles and calculate their movement
	for(unsigned int i = 0; i < particlesNo; i++)
		(*localParticles)[i]->moveEuler(dt);

	LEAVE	;
}

/**
 * Checks the boundary conditions and relocates the particles accordingly.
 */
void Simulation::checkBoundaryConditions()
{
	// create a local reference to the particles
	vector<Particle*>* localParticles = entityManager_->getParticles();

	if(simulationOptions_->boundaryCondition == boundaryConditionTypeNone)
		return;

	// use two for-loops for better perfomance (just one if statement!)
	if(simulationOptions_->boundaryCondition == boundaryConditionTypeSquare)
	{
		for(unsigned int i = 0; i < localParticles->size(); i++)
		{
			Particle* p = (*localParticles)[i];

			// use another loop -> no customization for 2d or 3d is needed
			for(int j = 0; j < p->position_.size() - 1; j++)
			{
				if(p->position_[j] > simulationOptions_->boundaryConditionSize)
					p->position_[j] *= -1;
			}
		}
	}
	else if(simulationOptions_->boundaryCondition == boundaryConditionTypeCircular)
	{
		for(unsigned int i = 0; i < localParticles->size(); i++)
		{
			if((*localParticles)[i]->position_.norm() > simulationOptions_->boundaryConditionSize)
			{
				(*localParticles)[i]->position_ *= -1;
			}
		}
	}
	//! Removes any particle that leaves the simulation area.
	//TODO: particles that are removed this way wont be printed in the output file! Their last known value should be printed for the rest of the simulation.
	else if(simulationOptions_->boundaryCondition == boundaryConditionOutFlow)
	{
		vector<Particle*>::iterator i;
		for(i = localParticles->begin(); i != localParticles->end(); ++i)
		{
			if((*i)->position_.norm() > simulationOptions_->boundaryConditionSize)
				localParticles->erase(i);
		}
	}
}

void Simulation::outputStatisticsHeader()
{
	cout << "#";
	if(simulationOptions_->outputParticleData)
	{
		vector<Particle*>* localParticles = entityManager_->getParticles();
		for(unsigned int i = 0; i < localParticles->size(); i++)
		{

			cout << "Particle " << i << " X\tY           \t";
		}
	}

	if(simulationOptions_->outputCloudData)
	{
		//output, cloud header
		vector<Cloud*>* localClouds = entityManager_->getClouds();
		for(unsigned int i = 0; i < localClouds->size(); i++)
		{
			cout << "Cloud " << i << " X\tY\t";
		}

		//output, mean cloud
		cout << "Center X\tY";
	}

	if(simulationOptions_->outputTotalEnergy)
		cout << "Total energy\t";
	if(simulationOptions_->outputKineticEnergy)
		cout << "Kin. energy\t";
	if(simulationOptions_->outputPotentialEnergy)
		cout << "Pot. energy\t";
	if(simulationOptions_->outputCOMKineticEnergy)
		cout << "COM Kin. En.\t";
	if(simulationOptions_->outputAvgCloudRadius)
	{
		vector<Cloud*>* localClouds = entityManager_->getClouds();
		for(unsigned int i = 0; i < localClouds->size(); i++)
		{
			cout << "C " << i << " Avg. Rad.\t";
		}
	}
	cout << endl;
}

//! Outputs the generated calculations.
/*!
 * Is also responsible for saving stuff in the history.
 */
void Simulation::outputStatistics()
{
	cout << setprecision(12);

	// print all particles' positions
	if(simulationOptions_->outputParticleData)
	{
		vector<Particle*>* localParticles = entityManager_->getParticles();
		for(unsigned int i = 0; i < localParticles->size(); i++)
		{
			cout << (*localParticles)[i]->position_[0] << "\t" << (*localParticles)[i]->position_[1] << "\t";
#ifdef THREE_DIMENSIONS
			cout << (*localParticles)[i]->position_[2] << "\t";
#endif
			(*localParticles)[i]->saveHistory();
		}
	}

	if(simulationOptions_->outputCloudData)
	{
		vector<Cloud*>* localClouds = entityManager_->getClouds();
		for(unsigned int i = 0; i < localClouds->size(); i++)
		{
			Vector pos = *(*localClouds)[i]->getPosition();
			cout << pos[0] << "\t" << pos[1] << "\t";
#ifdef THREE_DIMENSIONS
			cout << pos[2] << "\t";
#endif
			(*localClouds)[i]->saveHistory();
		}

		// calculate and print center of mass difference
		Vector mean;
		for(unsigned int i = 1; i < localClouds->size(); i++)
		{
			mean = *(*localClouds)[0]->getPosition();;
			Vector pos = *(*localClouds)[i]->getPosition();
			mean -= pos;
		}
		cout << mean[0] << "\t" << mean[1] << "\t";
#ifdef THREE_DIMENSIONS
		cout << mean[2] << "\t";
#endif
	}

	// output for additional infos
	if(simulationOptions_->outputTotalEnergy)
		cout << calculateTotalEnergy() << "\t";

	if(simulationOptions_->outputKineticEnergy)
		cout << calculateKineticEnergy() << "\t";

	if(simulationOptions_->outputPotentialEnergy)
		cout << calculatePotentialEnergy() << "\t";

	if(simulationOptions_->outputCOMKineticEnergy)
		cout << calculateCOMKineticEnergy() << "\t";

	cout << endl;
}

bool Simulation::checkConfigurationAndData()
{
	// check if a matrix has been loaded (only needed for harmonic interaction)
	if(simulationOptions_->harmonicForce)
	{
		try
		{
			// try to access every matrix element
			for(unsigned int i = 0; i < entityManager_->getParticlesNo(); i++)
			{
				for(unsigned int j = 0; j < entityManager_->getParticlesNo(); j++)
					simulationOptions_->harmonicInteractionMatrix(i,j);
			}
		}
		catch(exception)
		{
			ERROR("There was an error checking the harmonic matrix elements.")
			return false;
		}
	}

	// check for correct boundary condition
	if(simulationOptions_->boundaryCondition == boundaryConditionTypeUnknown)
	{
		ERROR("Unknown boundary condition, exiting...")
		return false;
	}

	// check for correct equation solver
	if(simulationOptions_->equationSolver == equationSolverTypeUnknown)
	{
		ERROR("Unknown equation solver, exiting...")
		return false;
	}

	return true;
}

/*!
 * Reads the command line arguments. The currently only supported option is the config file.
 * -c=config.txt
 */
bool Simulation::readCommandLineArguments(int argc, char** argv)
{
	ENTER
	/*
	 * There will always be at least one argument (the full path and executable)
	 * that's why we start the loop from 1.
	 */
	if(argc <= 1)
		return false;

	for(int i = 1; i < argc ; i++)
	{
		string s = argv[i];
		string identifier;	// saves the identifier, like 'c' for configfilename
		identifier = s.substr(1, s.find_first_of('=') - 1);

		if(identifier.compare("c") == 0)
		{
			// configuration file command line option
			if(this->readConfig(s.substr(3)) == false)
				return false;
		}
		else
		{
			// identifier not found, return false
			WARNING("Unknown command line argument: " + identifier)
			LEAVE
			return false;
		}
		identifier = "";
	}
	LEAVE
	return true;
}

/*!
 * Reads the configuration file which includes the normal simulation options like
 * number of timesteps or length of the timesteps as well as the particle setup.
 *
 * Methode is broken up into several sub-methods for better maintenance and readability
 *
 * WARNING: it is essential that any line that is not meant to be a header does not start with a '[' !
 */
bool Simulation::readConfig(string fileName)
{
	ENTER
	//Datei einlesen
	ifstream in;
	in.open(fileName.c_str(), ios::in);
	string line;
	vector<string>* dataStrings = new vector<string>();	// saves the read lines
	//dataStrings = new vector<string>();

	try
	{
		// check if file was opened successfully
		if(in.is_open())
		{
			// read file contents
			while(in.good())
			{
				// read the file line by line
				getline(in, line);

				// test if the line is a header
				if(line.substr(0,1).compare("[") == 0)
				{
					// if it does make sure the already saved data in dataStrings is processed
					// and clear the vector
					parseDataStrings(dataStrings);
					dataStrings->clear();
					// line will be added below
					dataStrings->push_back(line);
				}
				else if(line.substr(0,2).compare("//") == 0 || line.substr(0,1).compare("#") == 0)
				{
					// The read line is a comment. Just skip it.
				}
				else
				{
					// skip empty lines
					if(!line.empty())
						dataStrings->push_back(line);
				}
			}
			// eof reached (most likely, file could also be corrupted!)
			// make sure the last set of dataStrings is processed
			parseDataStrings(dataStrings);
		}
		else
		{
			ERROR("Could not open configuration file. Exiting simulation.")
			return false;
		}
	}
	catch(exception& e)
	{ /* Nothing special here, exception is used to make sure dataStrings gets deleted */ }

	delete dataStrings;
	in.close();
	LEAVE
	return true;
}

/*
 * Decides how the data is processed.
 */
bool Simulation::parseDataStrings(vector<string>* data)
{
	ENTER
	// check if there are any elements in the vector
	if(data->empty())
		return false;

	if(data->at(0).compare("[config]") == 0)
	{
		simulationOptions_ = SimulationOptions::fromStringList(data);
		LEAVE
		return true;
	}
	else if(data->at(0).compare("[entities]") == 0)
	{
		// skip the first line as icout << argv[i] << endl;t only containts information for parseDataStrings(...)
		for(size_t i = 1; i < data->size(); i++)
		{
			// check if the line starts with a '(particle)'
			if(data->at(0).find("[entities]") != string::npos)
				// go for it!
				entityManager_->addParticle(Particle::fromString(data->at(i)));
		}
		simulationOptions_->particleCount = entityManager_->getParticles()->size();
		LEAVE
		return true;
	}
	else
	{
		LEAVE
		return false;
	}
}

double Simulation::calculateKineticEnergy()
{
	double eKin = 0;

	vector<Particle*>* localParticles = entityManager_->getParticles();
	vector<Particle*>::iterator i;

	for(i = localParticles->begin(); i != localParticles->end(); ++i)
		eKin += 0.5 * (*i)->mass_ * (*i)->velocity_.squaredNorm();

	return eKin;
}

//! The calculation is only possible for two clouds!
double Simulation::calculateCOMKineticEnergy()
{
	// TODO: mass is always 1!
	double mass = entityManager_->getParticlesNo() * 1;
	Vector velocity = Vector(0,0);

	vector<Cloud*>* localClouds = entityManager_->getClouds();

	// check the number of clouds
	if(localClouds->size() != 2)
		return -1;

	Cloud* cloud1 = (*localClouds)[0];
	Cloud* cloud2 = (*localClouds)[1];

	velocity = (*cloud1->getVelocity()) - (*cloud2->getVelocity());
	//vector<Particle*>* localParticles = entityManager_->getParticles();
	//vector<Particle*>::iterator i;

	//for(i = localParticles->begin(); i != localParticles->end(); ++i)
	//{
	//	velocity += (*i)->velocity_;
	//	mass =+ (*i)->getMass();
	//}

	return 0.5 * mass * velocity.squaredNorm();
}

double Simulation::calculatePotentialEnergy()
{
	double ePot = 0;

	vector<Particle*>* localParticles = entityManager_->getParticles();

	for(unsigned int i = 0; i < localParticles->size(); i++)
	{
		ePot += globalInteractions_->computePotentialEnergy((*localParticles)[i],i);

		for(unsigned int j = 0; j < localParticles->size(); j++)
		{
			//if(j == i)
			//	continue;

			ePot += twoBodyInteractions_->computePotentialEnergy(i, (*localParticles)[i], j, (*localParticles)[j]);
		}
	}

	// calculate special interactions (which have their own inner loop)
	ePot += specialInteractions_->computePotentialEnergy();

	return ePot;
}

double Simulation::calculateTotalEnergy()
{
	return calculateKineticEnergy() + calculatePotentialEnergy();
}

} // End of namespace
