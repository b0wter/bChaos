/*
 * SpecialInteractions.cpp
 *
 *  Created on: 5 Aug 2011
 *      Author: b0wter
 */

#include "SpecialInteractions.h"

namespace Interactions
{

SpecialInteractions::SpecialInteractions(SimulationOptions* opt, EntityManager* manager)
{
	entityManager_ = manager;
	simulationOptions_ = opt;
	//harmonicInteractionMatrixInter = simulationOptions_->harmonicInteractionMatrixInter;
	//harmonicInteractionMatrixIntra = simulationOptions_->harmonicInteractionMatrixIntra;

	if(simulationOptions_->harmonicForce)
		this->harmonicForce = true;
}

SpecialInteractions::~SpecialInteractions()
{
	// TODO Auto-generated destructor stub
}

void SpecialInteractions::compute()
{
	if(harmonicForce)
		computeHarmonicInteraction();

}

/*!
 * Computes the force for the harmonic interaction, which consists of three parts.
 * F1 = k_ij (y_1 - x_1) ; F2 = w_ij (x_2 - x_1) ; F3 = w_ij (y_2 - y_1) ;
 */
void SpecialInteractions::computeHarmonicInteraction()
{
	//HACK: SpecialInteraction - harmonic force rausgeworfen!
	return;

	unsigned int N = entityManager_->getParticlesNo() / 2;

	vector<Particle*> cloud1 = *(*entityManager_->getClouds())[0]->getParticles();
	vector<Particle*> cloud2 = *(*entityManager_->getClouds())[1]->getParticles();

	// compute the forces for the particles of cloud 1
	for(unsigned int i = 0; i < N; i++)
	{
		for(unsigned int j = 0; j < N; j++)
		{
			Vector fi = harmonicInteractionMatrixInter(i,j) * ( *cloud2[j]->getPosition() - *cloud1[i]->getPosition() ) + harmonicInteractionMatrixIntra(i,j) * ( *cloud1[j]->getPosition() - *cloud1[i]->getPosition());
			cloud1[i]->addForce(&fi);

			//cerr << fi.x() << "," << fi.y() << endl;
		}
		//cerr << "nächstes Teilchen" << endl;
	}
	//cerr << "\nnächste Wolke" << endl;

	// compute the forces for the particles of cloud 2
	for(unsigned int j = 0; j < N; j++)
	{
		for(unsigned int i = 0; i < N; i++)
		{
			Vector fj = harmonicInteractionMatrixInter(i,j) * ( *cloud1[i]->getPosition() - *cloud2[j]->getPosition() ) + harmonicInteractionMatrixIntra(i,j) * ( *cloud2[i]->getPosition() - *cloud2[j]->getPosition());
			cloud2[j]->addForce(&fj);

			//cerr << fj.x() << "," << fj.y() << endl;
		}
		//cerr << "nächstes Teilchen" << endl;
	}
	//cerr << "\nnächster Zeitschritt" << endl;
}

//! Computes the potential energy for all special interactions
double SpecialInteractions::computePotentialEnergy()
{
	double energy = 0;

	if(harmonicForce)
		energy += computeHarmonicPotential();

	return energy;
}

/*!
 * Computes the potential energy of ALL particles for the harmonic potential:
 * V = 1/2 Sum[i,j] { k_ij (x_i - y_j)² + w_ij (x_i - x_j)² + w_ij (y_i - y_j)² }
 */
double SpecialInteractions::computeHarmonicPotential()
{
	//HACK: SpecialInteraction harmonic potential rausgeworfen
	return 0;

	double energy = 0;
	unsigned int noOfParticles = entityManager_->getParticlesNo();

	//vector<Particle*> particles = *(entityManager_->getParticles());
	//vector<Cloud*> clouds = *(entityManager_->getClouds());

	vector<Particle*> c1Particles = *(*entityManager_->getClouds())[0]->getParticles();
	vector<Particle*> c2Particles = *(*entityManager_->getClouds())[1]->getParticles();

	for(unsigned int i = 0; i < noOfParticles/2; i++)
	{
		for(unsigned int j = 0; j < noOfParticles/2; j++)
		{
			//energy += 0.5 * ( harmonicInteractionMatrixInter(i,j) * (*(c1Particles[i]->getPosition()) - *(c2Particles[j]->getPosition())) ); //+ harmonicInteractionMatrixIntra(i,j) * (*c1Particles[i]->getPosition() - *c1Particles[j]->getPosition()) + harmonicInteractionMatrixIntra(i,j) * (*c2Particles[i]->getPosition() - *c2Particles[j]->getPosition()) );
		}
	}

	return energy;
}

}
