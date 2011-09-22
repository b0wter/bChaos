/*
 * ParticleManager.cpp
 *
 *  Created on: 22 Dec 2010
 *      Author: b0wter
 */

#include "EntityManager.h"

namespace Entities
{

EntityManager::EntityManager()
{
	// TODO Auto-generated constructor stub
}

EntityManager::~EntityManager()
{
	ENTER
	/*
	 * Has to take care of freeing all the space that was allocated for the particles and clouds
	 */
	INFO("Deleting clouds")
	vector<Cloud*>::iterator iClouds;
	for(iClouds = clouds_.begin(); iClouds != clouds_.end(); ++iClouds)
		delete(*iClouds);
	clouds_.clear();

	INFO("Deleting particles")
	vector<Particle*>::iterator iParticles;
	for(iParticles = particles_.begin(); iParticles != particles_.end(); ++iParticles)
		delete(*iParticles);
	particles_.clear();

	LEAVE
}

//
// ----- Methoden -----
//

/*
 * Adds a new particle to the collection and creates the
 * associated cloud (if it doesnt already exist)
 */
bool EntityManager::addParticle(Particle* particle)
{
	ENTER
	// add the new particle
	particles_.push_back(particle);
	//check if the corresponding cloud has already been created
	bool cloudExists = false;
	for(size_t i = 0; i < clouds_.size(); i++)
	{
		if(clouds_[i]->getTag().compare(particle->getTag()) == 0)
		{
			clouds_[i]->addParticle(particle);
			cloudExists = true;
		}
	}

	if(!cloudExists)
	{
		// create a new cloud
		this->createCloud(particle->getTag())->addParticle(particle);
	}
	LEAVE
	return true;
}

/*
 * Creates a new cloud with the tag 'name'
 */
Cloud* EntityManager::createCloud(string name)
{
	ENTER
	Cloud* cloud = new Cloud(name);
	clouds_.push_back(cloud);
	LEAVE
	return cloud;
}

/*
 * Prints all particles with all properties into the terminal
 */
void EntityManager::particlesToTerminal()
{
	cerr << "--------------------------------------------------------------------------------" << endl;
	cerr << "Particles:" << endl;
	for(unsigned int i = 0; i < particles_.size(); i++)
		cerr << particles_[i]->toShortString() << endl;
	cerr << "--------------------------------------------------------------------------------" << endl;
}

/*
 * Prints all the clouds' names in the terminal
 */
void EntityManager::cloudsToTerminal(bool listChildren)
{
	vector<Cloud*>::iterator i;
	for(i = clouds_.begin(); i != clouds_.end(); ++i)
		(**i).toTerminal(listChildren);
}

vector<Particle*>* EntityManager::getParticles()
{
	return &particles_;
}

vector<Cloud*>* EntityManager::getClouds()
{
	return &clouds_;
}

unsigned int EntityManager::getParticlesNo()
{
	return particles_.size();
}

} // end of namespace
