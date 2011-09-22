/*
 * EntityManager.h
 *
 *	Der ParticleManager verwaltet alle Particles und die Clouds (die eigentlich nur die Arrays enthalten sollten?!)
 *	Die Clouds sind nur dafür da den Verwaltungsaufwand der verschiedenen Wechselwirkungen zu regeln und die Particle alle ohne zusätzliche if-Abfrage
 *	aufrufen zu können (Unterscheidung erfolgt über den Tag).
 *
 *  Created on: 22 Dec 2010
 *      Author: b0wter
 */

#ifndef ENTITYMANAGER_H_
#define ENTITYMANAGER_H_

#include "../Utilities/Utils.h"
#include "Particle.h"
#include "Cloud.h"

using namespace Utilities;

namespace Entities
{
class EntityManager {
public:
	EntityManager();
	virtual ~EntityManager();

	Cloud* createCloud(string name);				// creates a new cloud (is automatically created if encoutering a particle with an unknown tag)
	bool addParticle(Particle* particle);		// adds a new particle to the collection
	void particlesToTerminal();					// lists all particles in the terminal
	void cloudsToTerminal(bool listChildren);	// lists all clouds in the terminal
	vector<Particle*>* getParticles();
	vector<Cloud*>* getClouds();
	unsigned int getParticlesNo();

private:
	vector<Particle*> particles_;
	vector<Cloud*> clouds_;
};

} // end of namespace
#endif /* ENTITYMANAGER_H_ */
