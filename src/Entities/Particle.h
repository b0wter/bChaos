/*
 * Particle.h
 *
 *  Created on: 22 Dec 2010
 *      Author: b0wter
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_
#include "./Entity.h"
#include "./../Utilities/Utils.h"
#include "../Simulation/SimulationOptions.h"

using namespace std;
using namespace Utilities;

namespace Entities
{

class Particle : public Entity
{
public:
	Particle();
	virtual ~Particle();

	// -- Properties --
	void setPosition(Vector* position);
	void setVelocity(Vector* velocity);
	void setCurrentForce(Vector* force);
	void resetCurrentForce();
	void switchForce();
	void setLastForce(Vector* force);
	void setRadius(double radius);
	void setCharge(double charge);
	void setMass(double mass);
	void setInitialPosition(Vector* position);	// Unterschied zu SetPosition(...) ist der, dass der Ursprung mit gesetzt wird.

	Vector* getPosition();
	Vector* getVelocity();
	Vector* getCurrentForce();
	Vector* getLastForce();
	double getRadius();
	double getCharge();
	double getMass();

	void addPosition(Vector* translation);
	void addVelocity(Vector* acceleration);
	void addForce(Vector* force);

	// -- Methoden --
	static Particle* fromString(string s);     //Um einen Partikel aus den Strings in der Simulationskonfigurationsdatei zu erstellen

	Particle* copy();	//Erstellt eine Kopie dieser Instanz (mit anderer ID!)
	//! Moves the particle according to the Euler-method.
	/*!
	 * Note that it is not possible to implement moveVV(), since the Velocity-Verlet algorithm requires the force evaluation
	 * to be done betwen the computation of new positions and the new velocities.
	 */
	void moveEuler(double dt);
	void push(Vector* velocity);
	string toString(); 		// human readable output of the particle's properties
	string toShortString();	// gives a one line dump of the particle's properties
	void toCSV();
	//! Sets the amount of timesteps the particles saves in his history
	void setHistoryLength(unsigned int length);

	bool isTransformed;			// Saves the state of the position vectors. It it's true the coordinates are those for the diagonlizes Hamiltonian
	double mass_;
	double charge_;
	Vector currentForce_;
	Vector lastForce_;		// Buffer needed for the velocity verlet algorithm
	Vector origin_;
	double radius_;


private:
	void transformCoordinates(); // No argument needed since the current state is saved in 'isTransformed'.


};

} // end of namespace
#endif /* PARTICLE_H_ */
