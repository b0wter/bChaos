/*
 * Cloud.h
 *
 *	Beschreibt eine Teilchenwolke.
 *
 *  Created on: 25 Dec 2010
 *      Author: b0wter
 */

#ifndef CLOUD_H_
#define CLOUD_H_

#include "./../Utilities/Utils.h"
#include "./Particle.h"
#include "./Entity.h"

using namespace std;
using namespace Utilities;

namespace Entities
{
/**
 *	The cloud class was introduced to be able to quickly distinguish between the different particle clouds.
 *	Also offers the ability to calculate a center of mass velocity and position.
 */
class Cloud : public Entity
{
public:
	Cloud(string name);
	virtual ~Cloud();

	//Methoden
	void push(Vector* velocity);
	void addParticle(Particle* particle);
	void toTerminal(bool listChildren);

	//Properties
	Vector* getPosition();
	Vector* getVelocity();
	vector<Particle*>* getParticles();

private:
	vector<Particle*> particles_;
};

} // end of namespace
#endif /* CLOUD_H_ */
