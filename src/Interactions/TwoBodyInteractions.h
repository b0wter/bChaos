/*
 * TwoBodyInteractions.h
 *
 *  Created on: 19 Feb 2011
 *      Author: b0wter
 */

#ifndef TWOBODYINTERACTIONS_H_
#define TWOBODYINTERACTIONS_H_

/*
 *	Handles all two body interactions, like gravitation, coloumb, ...
 *	This class works with static methods, there is no reason to make any instances.
 *	(Thus the constructor is private.)
 */
#include "Interactions.h"
#include "../Entities/Particle.h"
#include "../Utilities/Constants.h"
#include "../Simulation/SimulationOptions.h"

using namespace Entities;
using namespace Utilities;

namespace Interactions
{

class TwoBodyInteractions: public Interactions::Interactions
{
public:
	virtual ~TwoBodyInteractions();
	TwoBodyInteractions(SimulationSpace::SimulationOptions* options);

	//! Computes all two body interactions
	void compute(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);
	//! Compute potential energy
	double computePotentialEnergy(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);

private:
	// functions
	inline void computeGravitationalForce(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);
	inline void computeCoulombForce(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);
	inline void computeHarmonicForce(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);
	inline void computePertubingForce(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);

	double computeGravitationalPotential(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);
	double computeCoulombPotential(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);
	double computeHarmonicPotential(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);
	double computePertubingPotential(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);
	//double computeHarmonicPotentialDiagonalised(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2);

	// members
	//vector<void (*)(unsigned int, Particle*, unsigned int, Particle*)> interactions_;
	bool coulombForce_;
	bool gravitationalForce_;
	bool harmonicForce_;
	bool pertubingForce_;

	double gravConstant_;

	MatrixXd harmonicInteractionMatrix;

	//MatrixXd harmonicInteractionMatrixInter;
	//MatrixXd harmonicInteractionMatrixIntra;
};

}

#endif /* TWOBODYINTERACTIONS_H_ */
