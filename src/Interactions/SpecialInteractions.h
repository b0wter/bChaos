/*
 * SpecialInteractions.h
 *
 *  Created on: 5 Aug 2011
 *      Author: b0wter
 */

#ifndef SPECIALINTERACTIONS_H_
#define SPECIALINTERACTIONS_H_

#include "Interactions.h"
#include "../Entities/EntityManager.h"
#include "../Simulation/SimulationOptions.h"
#include "../Utilities/Utils.h"

using namespace SimulationSpace;
using namespace Entities;
/*
 * Is made to handle all the interactions that require more than just the particle ids & pointers.
 * They have their own loop, so they are not called together with OneBodyInteractions and TwoBodyInteractions, which makes them slower!
 * Advantage over the other interactions is that they have access to all particles and simulation options at all times.
 * (Usefull for many-body-interactions)
 */
namespace Interactions
{

class SpecialInteractions : Interactions
{
public:
	SpecialInteractions(SimulationOptions* opt, EntityManager* manager);
	//! Differs from the other interactions, computes the forces for ALL particles!
	void compute();
	//! Differs from the other interactions, computes the potential energy of ALL particles!
	double computePotentialEnergy();

	virtual ~SpecialInteractions();

private:
	void computeHarmonicInteraction();
	double computeHarmonicPotential();

	EntityManager* entityManager_;
	SimulationOptions* simulationOptions_;
	bool harmonicForce;

	MatrixXd harmonicInteractionMatrixInter;
	MatrixXd harmonicInteractionMatrixIntra;

};


}
#endif /* SPECIALINTERACTIONS_H_ */


