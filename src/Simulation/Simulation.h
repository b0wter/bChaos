/*
 * Simulation.h
 *
 *  Created on: 22 Dec 2010
 *      Author: b0wter
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "../Entities/Entity.h"
#include "../Entities/Particle.h"
#include "../Entities/Cloud.h"
#include "../Entities/EntityManager.h"
#include "SimulationOptions.h"
#include "../Utilities/Constants.h"
#include "../Interactions/Interactions_Namespace.h"

using namespace Entities;
using namespace Utilities;

namespace SimulationSpace
{

class Simulation
{
public:
	Simulation(int argc, char** argv);
	virtual ~Simulation();

private:
	//Member
	EntityManager* entityManager_;			// saves all the particles and creates further lists for optimization
	SimulationOptions* simulationOptions_;	// saves all the parameters for the simulation, like timestep size

	// Interactions
	Interactions::TwoBodyInteractions* twoBodyInteractions_;
	Interactions::OneBodyInteraction* globalInteractions_;
	Interactions::SpecialInteractions* specialInteractions_;

	//Methoden
	//! Reads the command line options
	/*!
	 * Currently only the option "-c=filename" is supported to supply a configuration file.
	 * -c=filename	-	calls readConfig(...) for further processing
	 */
	bool readCommandLineArguments(int argc, char** argv);
	//! Reads the config file and calls apropriat submethodes
	/*!
	 * Opens filename to read the config parameters. Reads the file line by line collecting all line between headers
	 * (which are marked by brackets, e.g. [config]) and then calls parseDataStrings(...) for each list of strings.
	 * -
	 */
	bool readConfig(string fileName);
	//! Submethod of readConfig that decides wether the data-block is options or entities.
	bool parseDataStrings(vector<string>* data);
	void moveEuler(double dt, unsigned int step);			// performs a timestep using euler (needs to be the first step for most advanced algorithms)
	void moveVV(double dt, unsigned int step);				// performs a timestep using velocity-verlet
	//! Checks if all necessary parameters have been set and all required data loaded.
	bool checkConfigurationAndData();
	void (Simulation::*move)(double, unsigned int);
	INLINE void checkBoundaryConditions();							// checks if any particle is beyond the boundaries and relocates them accordingly
	INLINE void calculateForces(vector<Particle*>* particles);
	INLINE void outputStatistics();	// prints various data in the terminal
	double calculateTotalEnergy(); // computes the system's total energy
	double calculatePotentialEnergy();
	double calculateKineticEnergy();
};

} // end of namespace
#endif /* SIMULATION_H_ */
