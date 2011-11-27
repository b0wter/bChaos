/*
 * TwoBodyInteractions.cpp
 *
 *  Created on: 19 Feb 2011
 *      Author: b0wter
 */

#include "TwoBodyInteractions.h"

namespace Interactions
{

TwoBodyInteractions::TwoBodyInteractions(SimulationSpace::SimulationOptions* options)
{
	coulombForce_ = options->coulombForce;
	gravitationalForce_ = options->gravitationalForce;
	harmonicForce_ = options->harmonicForce;
	pertubingForce_ = options->pertubingForce;

	//TODO: outdated, should be removed soon
	harmonicInteractionMatrix = options->harmonicInteractionMatrix;
	if(options->harmonicForce)
		// modifiy the matrix
		for(int i = 0; i < options->harmonicInteractionMatrix.cols(); i++)
			for(int j = 0; j < options->harmonicInteractionMatrix.rows(); j++)
				if( (i >= options->harmonicInteractionMatrix.cols()/2 && j < options->harmonicInteractionMatrix.rows()/2) || (i < options->harmonicInteractionMatrix.cols()/2 && j >= options->harmonicInteractionMatrix.rows()/2))
					harmonicInteractionMatrix(i,j) = 0.5 * options->harmonicInteractionMatrix(i,j);

	for(int i = 0; i < options->harmonicInteractionMatrix.cols(); i++)
		for(int j = 0; j < options->harmonicInteractionMatrix.rows(); j++)
			harmonicInteractionMatrix(i,j) = 2 * harmonicInteractionMatrix(i,j);


	//cerr << harmonicInteractionMatrix << endl;

	//harmonicInteractionMatrixInter = options->harmonicInteractionMatrixInter;
	//harmonicInteractionMatrixIntra = options->harmonicInteractionMatrixIntra;

	gravConstant_ = 6.67428e-11;
}

TwoBodyInteractions::~TwoBodyInteractions()
{
	// TODO Auto-generated destructor stub
}

void TwoBodyInteractions::compute(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER	;

	if(gravitationalForce_)
		computeGravitationalForce(p1Index, p1, p2Index, p2);
	if(coulombForce_)
		computeCoulombForce(p1Index, p1,p2Index, p2);
	if(harmonicForce_)
		computeHarmonicForce(p1Index, p1, p2Index, p2);
	if(pertubingForce_)
		computePertubingForce(p1Index, p1, p2Index, p2);

	LEAVE	;
}

double TwoBodyInteractions::computePotentialEnergy(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER	;

	double Epot = 0;

	if(gravitationalForce_)
		Epot += computeGravitationalPotential(p1Index, p1, p2Index, p2);
	if(coulombForce_)
		Epot += computeCoulombPotential(p1Index, p1, p2Index, p2);
	if(harmonicForce_)
		Epot += 0.5 * computeHarmonicPotential(p1Index, p1, p2Index, p2);
	if(pertubingForce_)
		Epot += 0.5 * computePertubingPotential(p1Index, p1, p2Index, p2);

	return Epot;
	LEAVE	;
}


/*!
 * Calculates the Coulomb force for two particles and saves them in BOTH particle instnaces (Newton #3)
 * F = 1/(4*Pi*epsilon_0) * q_1 * q_2 / r^2
 * gives the absolut force, Direction is calculated by subtracting the particles position vectors.
 *  P_1 -> P_2 => r_2 - r_1 
 *  Note that the two int parameters are not needed! They are only to privede the same signature for all interactions so
 *  they can be stored in a vector of function pointers!
 */
void TwoBodyInteractions::computeCoulombForce(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER

	Vector direction = *(p2->getPosition()) - *(p1->getPosition());
	double distance = direction.squaredNorm();
	double force = 1/(4 * Constants::pi * Constants::epsilon_0) * p1->getCharge() * p2->getCharge() / distance;
	direction.normalize();
	direction *= force;

	p1->addForce(&direction);
	direction *= -1;
	p2->addForce(&direction);

	LEAVE
}

/*!
 * Calculates the gracitational force and saves them in BOTH particle instances (Newton #3)
 * F = gamma m_1 m_2 / r^2
 */
void TwoBodyInteractions::computeGravitationalForce(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER	;

	Vector direction = (*(p2->getPosition()) - *(p1->getPosition()));
	double distance = direction.squaredNorm();
	double force = p1->getMass() * p2->getMass() / distance;
	direction.normalize();
	direction *= force;

	p1->addForce(&direction);
	direction *= -1;
	p2->addForce(&direction);

	LEAVE	;
}

/*!
 * Calculates the harmonic potential's 0.5 * ( M_ij * (r_i + r_j)² ) force and saves it in
 * both particle instances. 
 * NOTE: currently it is assumed that F_ij is equal to F_ji !
 */
void TwoBodyInteractions::computeHarmonicForce(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER	;

	//Vector force = harmonicInteractionMatrix(p1Index, p2Index) * ( *(p2->getPosition()) - *(p1->getPosition()) );
	Vector force = 2*harmonicInteractionMatrix(p1Index, p2Index) * ( *(p2->getPosition()) - *(p1->getPosition()) );
	Vector force1 = force; Vector force2 = -1 * force;

	p1->addForce(&force1); p2->addForce(&force2);

	LEAVE	;
}

/*!
 * Calculates the force of the potential: ( M_ij * (r_i + r_j)^4 ) and saves it in
 * both particle instances.
 * NOTE: currently it is assumed that F_ij is equal to F_ji !
 */
void TwoBodyInteractions::computePertubingForce(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER	;

	// check the clouds
	int p1Cloud = -1, p2Cloud = -1;

	if(p1Index < (unsigned int)harmonicInteractionMatrix.rows())
		p1Cloud = 1;
	else
		p1Cloud = 2;

	if(p2Index < (unsigned int)harmonicInteractionMatrix.rows())
		p2Cloud = 1;
	else
		p2Cloud = 2;

	// ^4 part
	if(p1Cloud != p2Cloud)
	{
		Vector difference = ( *(p2->getPosition()) - *(p1->getPosition()) );
		difference = (difference.cwise() * difference).cwise() * difference;
		Vector force = 2 * harmonicInteractionMatrix(p1Index, p2Index) * difference;
		Vector force1 = force; Vector force2 = -1 * force;

		p1->addForce(&force1); p2->addForce(&force2);
	}
	// ^2 part
	else
	{
		Vector force = harmonicInteractionMatrix(p1Index, p2Index) * ( *(p2->getPosition()) - *(p1->getPosition()) );
		Vector force1 = force; Vector force2 = -1 * force;

		p1->addForce(&force1); p2->addForce(&force2);
	}

	LEAVE	;
}

double TwoBodyInteractions::computeGravitationalPotential(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER	;

	Vector v1 = *(p1->getPosition()); Vector v2 = *(p2->getPosition());
	double energy = gravitationalForce_ * p2->getMass() / (v1 - v2).norm();

	LEAVE	;
	return energy;
}

double TwoBodyInteractions::computeCoulombPotential(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER	;

	//throw "Not implemented!";

	LEAVE	;

	return 0;
}

double TwoBodyInteractions::computeHarmonicPotential(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER	;

	Vector v1 = *(p1->getPosition()); Vector v2 = *(p2->getPosition());

	//double energy = 0.5 * p1->getMass() * harmonicInteractionMatrix(p1Index,p2Index) * (v1 - v2).squaredNorm();
	double energy = p1->getMass() * harmonicInteractionMatrix(p1Index,p2Index) * (v1 - v2).squaredNorm();

	LEAVE	;
	return energy;
}

double TwoBodyInteractions::computePertubingPotential(unsigned int p1Index, Particle* p1, unsigned int p2Index, Particle* p2)
{
	ENTER	;

	Vector v1 = *(p1->getPosition()); Vector v2 = *(p2->getPosition());

	double energy = 0.5 * p1->getMass() * harmonicInteractionMatrix(p1Index,p2Index) *  (v1 - v2).squaredNorm() * (v1 - v2).squaredNorm();

	LEAVE	;
	return energy;
}

} // end of namespace
