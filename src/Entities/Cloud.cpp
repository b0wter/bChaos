/*
 * Cloud.cpp
 *
 *  Created on: 25 Dec 2010
 *      Author: b0wter
 */

#include "Cloud.h"

namespace Entities
{

Cloud::Cloud(string name)
{
	tag_ = name;
	id_ = (size_t)this;
}

Cloud::~Cloud()
{
	ENTER

	LEAVE
}


//
// ----------------- Methoden -------------------
//

void Cloud::push(Vector* velocity)
{
	//Alle particles schubsen!
	for(size_t i = 0; i < particles_.size(); i++)
		particles_[i]->push(velocity);
}

void Cloud::addParticle(Particle* particle)
{
	particles_.push_back(particle);
}

void Cloud::toTerminal(bool listChildren)
{
	cerr << endl;
	cerr << "Cloud: " << this->getTag() << endl;
	vector<Particle*>::iterator i;
	if(listChildren)
		for(i = particles_.begin(); i != particles_.end(); ++i)
			cerr << (**i).toShortString() << endl;
	if(!listChildren)
		cerr << endl;
}

//
// ----------------- Properties -------------------
//
Vector* Cloud::getPosition()
{
	//Schwerpunktskoordinate berechnen
	position_ = Vector::Zero();
	for(size_t i = 0; i < particles_.size(); i++)
	{
		position_ += (*particles_[i]->getPosition());
	}
	position_ /= particles_.size();
	return &position_;
}

Vector* Cloud::getVelocity()
{
	Vector* groupVelocity = new Vector();
	for(size_t i = 0; i < particles_.size(); i++)
	{
		*groupVelocity += *particles_[i]->getVelocity();
	}
	*groupVelocity /= particles_.size();
	return groupVelocity;
}

vector<Particle*>* Cloud::getParticles()
{
	return &particles_;
}

} // end of namespace
