/*
 * Entity.cpp
 *
 *  Created on: 6 Jan 2011
 *      Author: b0wter
 */

#include "Entity.h"

namespace Entities
{

Entity::Entity()
{
	history_ = 0;
	saveHistory_ = false;
	tag_ = "<no tag>";
	id_ = (size_t)this;
	historyIndex_ = 0;

}

Entity::~Entity()
{
	ENTER

	delete [] history_;

	LEAVE
}

string Entity::historyToString()
{
	stringstream oss;
	for(unsigned int i = 0; i < historyLength_; i++)
	{
		oss << history_[i][0] << "\t" << history_[i][1];
#ifdef THREE_DIMENSIONS
		oss << "\t" << history[i][2];
#endif
		oss << endl;
	}
	return oss.str();
}

Vector Entity::getHistoryElement(unsigned int index)
{
	return history_[index];
}

void Entity::push(Vector* velocity)
{
	//there should be dragons!
	throw "Entity::push() - sollte nie direkt aufgerufen werden!";
}

void Entity::saveHistory()
{
	if(saveHistory_)
	{
		history_[historyIndex_] = position_;
		historyIndex_++;
	}
}

//! Enables the history saving.
/*!
 * It is important to note, that the initial positions are also written to the data file thus the number of timesteps
 * is effectivly 1 unit larger then the number of timesteps computed.
 */
void Entity::enableHistory(unsigned int historyLength)
{
	history_ = new Vector[historyLength+1];
	historyLength_ = historyLength+1;
	saveHistory_ = true;
}

//
// ----------------- Properties -----------------
//
Vector* Entity::getPosition()
{
	//sollte nie aufgerufen werden!
	throw "Entity::getPosition() - sollte nie direkt aufgerufen werden!";
}

Vector* Entity::getVelocity()
{
	throw "Entity::getVelocity() - sollte nie direkt aufgerufen werden!";
}

string Entity::getTag()
{
	return tag_;
}

void Entity::setTag(string newTag)
{
	tag_ = newTag;
}

size_t Entity::getID()
{
	return id_;
}

} // end of namespace
