/*
 * Entity.h
 *
 *	Ist die Basisklasse für alle Objekte, die bewegt werden können, also Particle und Cloud.
 *
 *  Created on: 6 Jan 2011
 *      Author: b0wter
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "./../Utilities/Utils.h"

using namespace std;
using namespace Utilities;

namespace Entities
{

/**
 *	Base class for clouds and particles.
 */
class Entity
{
public:
	Entity();
	virtual ~Entity();
	virtual void push(Vector* velocity);	//Setzt das Objekt in Bewegung
	//! Saves the entities current state in it's history.
	void saveHistory();
	void enableHistory(unsigned int historyLength);
	string historyToString();
	Vector getHistoryElement(unsigned int index);

	//Properties
	virtual Vector* getPosition();
	virtual Vector* getVelocity();

	string getTag();
	void setTag(string newTag);
	size_t getID();

	Vector position_;				// center of mass, for a cloud
	Vector velocity_;				// center of mass - movement for a cloud
	string tag_;
	size_t id_;

protected:
	//! Activates the saving of the particle's history.
	bool saveHistory_;
	//! Sets the maximum number of positions saved.
	unsigned int historyLength_;
	//! Pointer to the first element of the entities history array.
	Vector* history_;
	//! Current position in the history.
	/*!
	 * Increases each time a new element is added. If the index is > size then the
	 */
	unsigned int historyIndex_;
};

} // end of namespace

#endif /* ENTITY_H_ */
