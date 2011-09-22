/*
 * GlobalInteractions.h
 *
 *  Created on: 19 Feb 2011
 *      Author: b0wter
 */

#ifndef GLOBALINTERACTIONS_H_
#define GLOBALINTERACTIONS_H_

/*
 *	Handles all global interactions, e.g. a global gravity
 *	This class works with static methods, there is no reason to make any instances.
 *	(Thus the constructor is private.)
 *
 *	Currently supported global interactions:
 *		- gravity (simple acceleration)
 */
#include "Interactions.h"
#include "../Entities/Entity.h"
#include "../Entities/Particle.h"
#include "../Simulation/SimulationOptions.h"

using namespace Entities;
using namespace Utilities;

namespace Interactions
{

class OneBodyInteraction: public Interactions::Interactions
{
public:
	OneBodyInteraction(SimulationSpace::SimulationOptions* options);
	virtual ~OneBodyInteraction();

	//! Computes all global interactions
	/*!
	 * This function starts the computation for all global interactions.
	 * So far the only supported option is global gravity.
	 */
	void compute(Particle* p, unsigned int pIndex);
	//! Calculate the potential energy.
	/*!
	 * It is used to check the system's energy behaviour over time (one check at the start, one at the end).
	 */
	double computePotentialEnergy(Particle* p, unsigned int pIndex);

private:
	bool globalGravity_;
	bool diagonalizedHarmonicForce_;

	//! Matrix for the diagonalised harminc interaction
	MatrixXd diagonalizedHarmonicInteractionMatrix;

	//! Calculates the global gravity.
	inline void computeGlobalGravity(Particle* p, unsigned int pIndex);
	inline void computeDiagonalizedHarmonicForce(Particle* p, unsigned int pIndex);

	double computeDiagonalizeHarmonicPotential(Particle* p, unsigned int pIndex);

	//! The strength of the acceleration (meter/second²)
	double globalGravityAcceleration;
	//! The global gravity's direction.
	Vector globalGravityDirection;

	// constants
	static const double gravitationalConstant = 6.67428E-11;
};


} // end of namespace

#endif /* GLOBALINTERACTIONS_H_ */
