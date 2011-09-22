/*
 * GlobalInteractions.cpp
 *
 *  Created on: 19 Feb 2011
 *      Author: b0wter
 */

#include "OneBodyInteractions.h"

namespace Interactions
{

OneBodyInteraction::OneBodyInteraction(SimulationSpace::SimulationOptions* options)
{
	globalGravityAcceleration = options->globalGravityAcceleration;
	globalGravityDirection = options->globalGravityDirection;
	diagonalizedHarmonicForce_ = options->diagonalizedHarmonicForce;
	diagonalizedHarmonicInteractionMatrix = options->diagonalizedHarmonicInteractionMatrix;
}

OneBodyInteraction::~OneBodyInteraction()
{
	// TODO Auto-generated destructor stub
}

void OneBodyInteraction::compute(Particle* p, unsigned int pIndex)
{
	if(globalGravity_)
		computeGlobalGravity(p, pIndex);
	if(diagonalizedHarmonicForce_)
		computeDiagonalizedHarmonicForce(p, pIndex);
}

/*!
 *	Calculate a global acting gravitational (=attractive) force. This is implementened merely
 *	for testing purposes, not for actual use. It does not even take any distances into account.
 */
void OneBodyInteraction::computeGlobalGravity(Particle* p, unsigned int pIndex)
{
	double force = p->getMass() * globalGravityAcceleration;
	Vector direction = (force * globalGravityDirection);
	p->addForce(&direction);
}

/*!
 * Computes the harmonic force for the diagonalizes coordinates and interaction matrix.
 * H = 1/2 m w r² -> F = - m w r 				(f = - grad V)
 */
void OneBodyInteraction::computeDiagonalizedHarmonicForce(Particle* p, unsigned int pIndex)
{
	ENTER	;

	Vector force = -1 * p->getMass() * diagonalizedHarmonicInteractionMatrix(pIndex, pIndex) * *(p->getPosition());
	p->addForce(&force);

	// Alte Version:
	/*
	//Vector direction = p->getPosition().cwise(-1 * p->getMass() * diagonalizedHarmonicInteractionMatrix(pIndex,pIndex));
	double force = -1 * p->getMass() * diagonalizedHarmonicInteractionMatrix(pIndex,pIndex);
	Vector direction = (p->position_ * force);
	p->addForce(&direction);
	*/

	LEAVE;
}

//! Checks what interactions are used and compute the respective potential energy for the given particle
double OneBodyInteraction::computePotentialEnergy(Particle* p, unsigned int pIndex)
{
	ENTER	;
	double potential = 0;
	if(diagonalizedHarmonicForce_)
		potential += computeDiagonalizeHarmonicPotential(p, pIndex);

	LEAVE	;
	return potential;
}

//! Computes the diagonalized harmonic potential for the given particle ( H = 0.5 m w_i r_i² )
double OneBodyInteraction::computeDiagonalizeHarmonicPotential(Particle* p, unsigned int pIndex)
{
	ENTER	;
	double potential = 0.5 * p->getMass() * diagonalizedHarmonicInteractionMatrix(pIndex, pIndex) * p->position_.squaredNorm();
	LEAVE	;
	return potential;
}

} // end of namespace
