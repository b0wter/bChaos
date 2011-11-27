/*
 * SimulationOptions.h
 *
 *  Created on: 21 Jan 2011
 *      Author: b0wter
 */

#ifndef SIMULATIONOPTIONS_H_
#define SIMULATIONOPTIONS_H_

#include "../Utilities/Utils.h"

namespace SimulationSpace
{

using namespace Utilities;

/**
 * Saves all the simulation parameters, like timestep size, number of timesteps, ...
 */
class SimulationOptions
{
public:
	SimulationOptions();
	virtual ~SimulationOptions();

	//! Creates an instance of simulationOptions from a list of strings.
	static SimulationOptions* fromStringList(vector<string>* strings);

	//! Lists all options in the terminal.
	void toTerminal();

	//! Create a (large) string that holds all simulation information.
	/*!
	 * Basically gives the string that is printed in the terminal by toTerminal(). 
	 * If the bool is true every new line starts with a '#' to indicate a comment. Useful for the output in a data file.
	 */
	string toString(bool commentary = false);

	//! The size (lenght) of a timestep.
	double timeStepSize;
	//! The number of total timesteps
	/*!
	 * Note that the output is also generatet for timestep zero, to include the starting positions/velocities.
	 */
	unsigned int timeStepNo;
	//! Enable simulation of a global gravitation
	bool globalGravity;
	//! The direction in which the global gravity pulls the entities.
	/*!
	 * Note that this is a very simple implementation. It simply accelerates the entities with a given acceleration
	 * (globalGravityAcceleration) and does not take any distances into account.
	 */
	Vector globalGravityDirection;
	//! The acceleration by the global gravitation. (e.g. 9.81 for earth)
	/*!
	 * Note that this is a very simple implementation. It simply accelerates the entities with a given acceleration
	 * (globalGravityAcceleration) and does not take any distances into account.
	 */
	double globalGravityAcceleration;
	//! The algorithm used to calculate the equations of motion.
	/*!
	 * Currently (21.02) it only supports two methods, the Euler and the Velocity-Verlet algorithm. 
	 * Note that the first timestep is always calculated with the Euler methode since VV needs to know information
	 * about the last timestep.
	 */
	equationSolverType equationSolver;
	//! The size of the "simulation area".
	/*!
	 * Gives the size (radius for circular, or 1/2 the edge length for square boundary conditions) of the area in which
	 * the simulation takes place. Any entity that leaves the are enters it on the opposit with unchanged velocities.
	 */
	double boundaryConditionSize;
	//! The type of boundary condition (circular, square).
	/*!
	 * Saves the type of boundary conditions used. Possible values are square, circular, outflow, none and unknown. 
	 * Unknown is only used to indicate that something went wrong while reading config file or converting in/to strings.
	 */
	boundaryConditionType boundaryCondition;

	//! Enable simulation of the gravitational force.
	bool gravitationalForce;
	//! Enable simulation of the coulomb force.
	bool coulombForce;
	//! Enable simulation of the harmonic force.
	/*!
	 * The harmonic potential is a rather special force. It simulates the coupling of the particles that acts like a harmonic spring.
	 * It is calculated as: (to be read like latex)
	 * V = Sum_(i!=j) * M_ij * (r_i - r_j)^2 
	 * F = 2 * M_ij * r_ij
	 */
	bool harmonicForce;
	//! Necessary for the harmonic force.
	/*!
	 * Describes the strength of the harmonic interaction for each particle with every other particle.
	 */
	MatrixXd harmonicInteractionMatrix;
	MatrixXd pertubingInteractionMatrix;
	//MatrixXd harmonicInteractionMatrixInter;
	//MatrixXd harmonicInteractionMatrixIntra;

	//! Enable the simulaton of the diagonalized harmonic force.
	bool diagonalizedHarmonicForce;
	//! Necessary for the diagonalized harmonic force
	MatrixXd diagonalizedHarmonicInteractionMatrix;
	//! Enables the simulation of the ^4 potential (using the same matrix as the harmonic potential)
	bool pertubingForce;
	//! Enables the particles to save their own position history in an array for further analyzing.
	bool saveParticleHistory;
	//! Enables the clouds to save their own position history in an array for further analyzing.
	bool saveCloudHistory;
	//! Enables output of particle data
	bool outputParticleData;
	//! Enables output of cloud data
	bool outputCloudData;
	//! Enables output of total energy each timesteap
	bool outputTotalEnergy;
	//! Enables output of kinetic energy on each timesteap
	bool outputKineticEnergy;
	//! Enables output of potential energy on each timestep
	bool outputPotentialEnergy;
	//! Enables output of the COM's kinetic energy
	bool outputCOMKineticEnergy;
	//! Enables output of the clouds average radius (averaged over all particle distances)
	bool outputAvgCloudRadius;
	//! Necessary for the harmonic interaction
	unsigned int particleCount;

};

} // end of namespace

#endif /* SIMULATIONOPTIONS_H_ */
