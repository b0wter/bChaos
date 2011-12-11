/*
 * SimulationOptions.cpp
 *
 *  Created on: 21 Jan 2011
 *      Author: b0wter
 */

#include "SimulationOptions.h"


namespace SimulationSpace
{

/*
 * Generates some hard-coded values
 */
SimulationOptions::SimulationOptions()
{
	ENTER

	timeStepNo = 0;
	timeStepSize = 1;

	// disable any global interaction
	globalGravity = false;
	globalGravityAcceleration = 0.0;
	globalGravityDirection = Vector(0,0);

	// disable any two-body-interactions and make sure everythin is initialized, just in case.
	coulombForce = false;
	gravitationalForce = false;
	harmonicForce = false;
	diagonalizedHarmonicForce = false;
	pertubingForce = false;
	//harmonicInteractionMatrix = Matrix3d::Zero();

	// disable the history
	saveCloudHistory = false;
	saveParticleHistory = false;

	// output
	outputParticleData = false;
	outputCloudData = false;
	outputTotalEnergy = false;
	outputKineticEnergy = false;
	outputPotentialEnergy = false;
	outputAvgCloudRadius = false;
	outputCOMKineticEnergy = false;
	outputMomentum = false;

	// set the default euqationSolver to be velocity-verlet
	equationSolver = equationSolverTypeUnknown;

	// set the boundary conditions to none since the system scale is unknown
	boundaryCondition = boundaryConditionTypeUnknown;
	boundaryConditionSize = -1.0;

	LEAVE
}

SimulationOptions::~SimulationOptions()
{
	ENTER

	// Nothing to be done here.

	LEAVE
}

/**
 * Creates a new instance of SimulationOptions from a given list of strings
 */
SimulationOptions* SimulationOptions::fromStringList(vector<string>* strings)
{
	ENTER
	/*
	 * Check if there is any data in the vector and
	 * check if the first element is names "[config]"
	 */
	if(strings->empty())
	{
		LEAVE
		return NULL;
	}
	if(strings->at(0) != "[config]")
	{
		LEAVE
		return NULL;
	}

	SimulationOptions* opt = new SimulationOptions();
	vector<string>::iterator i;

	/*
	 * Format looks like:
	 * timeStepNo=1000;
	 */
	string identifier; string value;
	for(i = strings->begin() + 1; i != strings->end(); ++i) // Advance the iterator by 1 (frist element ist unusable)
	{
		unsigned int indexEqual = i->find_first_of('=');

		identifier = i->substr(0,indexEqual);
		value = i->substr(indexEqual + 1);
		value.erase(value.size() - 1);

		/*
		 * Check what kind of Information was extraced :]
		 */
		if(identifier == "timeStepNo")
			opt->timeStepNo = uintFromString(value);
		else if(identifier == "timeStepSize")
			opt->timeStepSize = doubleFromString(value);
		else if(identifier == "globalGravity")
			opt->globalGravity = (bool)intFromString(value);
		else if(identifier == "globalGravityDirection")
			opt->globalGravityDirection = vectorFromString(value);
		else if (identifier == "globalGravityAcceleration")
			opt->globalGravityAcceleration = doubleFromString(value);
		else if (identifier == "equationSolver")
			opt->equationSolver = equationSolverFromString(value);
		else if (identifier == "boundaryCondition")
			opt->boundaryCondition = boundaryConditionFromString(value);
		else if (identifier == "coulombForce")
			opt->coulombForce = (bool)intFromString(value);
		else if (identifier == "gravitationalForce")
			opt->gravitationalForce = (bool) intFromString(value);
		else if (identifier == "harmonicForce")
			opt->harmonicForce = (bool)intFromString(value);
		else if (identifier == "diagonalizedHarmonicForce")
			opt->diagonalizedHarmonicForce = (bool)intFromString(value);
		else if (identifier == "saveParticleHistory")
			opt->saveParticleHistory = (bool)intFromString(value);
		else if (identifier == "saveCloudHistory")
			opt->saveCloudHistory = (bool)intFromString(value);
		else if (identifier == "outputParticleData")
			opt->outputParticleData = (bool)intFromString(value);
		else if (identifier == "outputCloudData")
			opt->outputCloudData = (bool)intFromString(value);
		else if (identifier == "outputTotalEnergy")
			opt->outputTotalEnergy = (bool)intFromString(value);
		else if (identifier == "outputKineticEnergy")
			opt->outputKineticEnergy = (bool)intFromString(value);
		else if (identifier == "outputPotentialEnergy")
			opt->outputPotentialEnergy = (bool)intFromString(value);
		else if (identifier == "outputCOMKineticEnergy")
			opt->outputCOMKineticEnergy = (bool)intFromString(value);
		else if (identifier == "outputMomentum")
			opt->outputMomentum = (bool)intFromString(value);
		else if (identifier == "outputAvgCloudRadius")
			opt->outputAvgCloudRadius = (bool)intFromString(value);
		else if (identifier == "pertubingForce")
			opt->pertubingForce = (bool)intFromString(value);
		else if (identifier == "harmonicInteractionMatrix")
			{
				if(matrixFromFile(&(opt->harmonicInteractionMatrix), value) == false)
					ERROR("Error reading the harmonic interaction matrix file: " + value) ;
			}
		else if (identifier == "diagonalizedHarmonicInteractionMatrix")
			{
				if(matrixFromFile(&(opt->diagonalizedHarmonicInteractionMatrix), value) == false)
					ERROR("Error reading the diagonalized harmonic interaction matrix file: " + value) ;
			}
		else if (identifier == "pertubingInteractionMatrix")
			{
				if(matrixFromFile(&(opt->pertubingInteractionMatrix), value) == false)
					ERROR("Error reading the pertubing interaction matrix file: " + value) ;
			}
//		else if (identifier == "harmonicInteractionMatrixInter")
//			{
//				if(matrixFromFile(&(opt->harmonicInteractionMatrixInter), value) ==false)
//					ERROR("Error reading the diagonalized harmonic interaction matrix (inter cloud) file: " + value) ;
//			}
//		else if (identifier == "harmonicInteractionMatrixIntra")
//			{
//				if(matrixFromFile(&(opt->harmonicInteractionMatrixIntra), value) ==false)
//					ERROR("Error reading the diagonalized harmonic interaction matrix (intra cloud) file: " + value) ;
//			}
		else
			// no suitable identifier found :-O
			// just give the information and continue
			WARNING("Configuration file identifier is unknown! (" + identifier + ")") ;
	}

	LEAVE
	return opt;
}

/**
 * Prints all the simulation's settings in the terminal
 */
void SimulationOptions::toTerminal()
{
	cerr << toString(false);
}

string SimulationOptions::toString(bool commentary)
{
	string comment = "";
	if(commentary)
		comment = "#";

	stringstream oss;
	oss << endl;
	oss << comment << "Configuration:" << endl;
	oss << comment << left << setw(30) << "timeStepNo:" << setw(15) << right << this->timeStepNo << endl;
	oss << comment << left << setw(30) << "timeStepSize:" << setw(15) << right << this->timeStepSize << endl;
	oss << comment << left << setw(30) << "equationSolver:" << setw(15) << right << equationSolverTypeName(this->equationSolver) << endl;
	oss << comment << left << setw(30) << "boundaryCondition:" << setw(15) << right << boundaryConditionToString(this->boundaryCondition) << endl;
	if(boundaryCondition != boundaryConditionTypeNone)
		oss << comment << left << setw(30) << "boundarySize:" << setw(15) << right << boundaryConditionSize << endl;
	oss << endl;
	oss<< comment << "Output:" << endl;
	oss << comment << left << setw(30) << "saveParticleHistory:" << setw(15) << right << saveParticleHistory << endl;
	oss << comment << left << setw(30) << "saveCloudHistory:" << setw(15) << right << saveCloudHistory << endl;
	oss << comment << left << setw(30) << "outputParticleData:" << setw(15) << right << outputParticleData << endl;
	oss << comment << left << setw(30) << "outputCloudData:" << setw(15) << right << outputCloudData << endl;
	oss << comment << left << setw(30) << "outputTotalEnergy:" << setw(15) << right << outputTotalEnergy << endl;
	oss << comment << left << setw(30) << "outputKineticEnergy:" << setw(15) << right << outputKineticEnergy << endl;
	oss << comment << left << setw(30) << "outputPotentialEnergy:" << setw(15) << right << outputPotentialEnergy << endl;
	oss << endl;
	oss << comment << "Interactions:" << endl;
	oss << comment << left << setw(30) << "globalGravity:" << setw(15) << right << this->globalGravity << endl;
	if(globalGravity) {
		oss << comment << left << setw(30) << "globalGravityAcceleration:" << setw(15) << right << this->globalGravityAcceleration << endl;
		oss << comment << left << setw(30) << "globalGravityDirection:" << setw(15) << right << vectorToString(&globalGravityDirection) << endl; }
	oss << comment << left << setw(30) << "coulombForce:" << setw(15) << right << this->coulombForce << endl;
	oss << comment << left << setw(30) << "gravitationalForce:" << setw(15) << right << this->gravitationalForce << endl;
	oss << comment << left << setw(30) << "harmonicForce:" << setw(15) << right << this->harmonicForce << endl;
	oss << comment << left << setw(30) << "diagonalHarmonicForce:" << setw(15) << right << this->diagonalizedHarmonicForce << endl;
	if(this->harmonicForce)
		oss << comment << left << setw(30) << "harmonic interaction matrix dim:" << setw(15) << right << this->harmonicInteractionMatrix.rows() << "x" << this->harmonicInteractionMatrix.cols() << endl;
	if(this->diagonalizedHarmonicForce)
		oss << comment << left << setw(30) << "harmonic interaction matrix dim:" << setw(15) << right << this->diagonalizedHarmonicInteractionMatrix.rows() << "x" << this->diagonalizedHarmonicInteractionMatrix.cols() << endl;
	oss << comment << left << setw(30) << "pertubingForce:" << setw(15) << right << this->pertubingForce << endl;
	//cerr << left << setw(30) << "harmonicInteractionMatrix:" << endl;
	//cerr << this->harmonicInteractionMatrix << endl;
	oss << endl;

	return oss.str();
}

} // end of namespace
