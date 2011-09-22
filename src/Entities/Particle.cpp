/*
 * Particle.cpp
 *
 *  Created on: 22 Dec 2010
 *      Author: b0wter
 */

#include "Particle.h"

namespace Entities
{

Particle::Particle()
{
	position_ = Vector::Zero();
	origin_ = Vector::Zero();
	velocity_ = Vector::Zero();
	currentForce_ = Vector::Zero();
	lastForce_ = Vector::Zero();
	mass_ = 1;
	charge_ = 0;
	radius_ = 1;
	historyLength_ = 10000;

	//ID generieren, wird aus der Adresse der Klasse erstellt
	id_ = (size_t)this;
}

Particle::~Particle()
{

}

//
// ----------------- Methoden -------------------
//

/*!
 * Erstellt aus einem String in der Simulationskonfigurationsdatei einen Partikel.
 * Mögliche Parameter sind:
 * Position (p), Radius (r), Masse (m), Ladung (c), Geschwindigkeit (v), Tag (t)
 * Bsp.:
 * (particle) p=0,0,1; r=1; m=2.5; c=0; v=0,0,-1;t=1;
 *
 * ACHTUNG: Maximale Zeilenlänge ist 255 Zeichen!
 * Einschränkungen:
 * - Zeilen die mit // beginnen werden als Kommentare gewertet (nur am Anfang verwenden!)
 * - Parameter müssen mit einem ; abgeschlossen werden
 */
Particle* Particle::fromString(string line)
{
	Particle* newParticle;
	newParticle = new Particle();

	int current = -1, next = -1; 	// Indizes für die '=' und ';'
	const int paramWidth = 1; 	// Zur besseren Erweiterbarkeit
	double valuedouble;			// Speichert die ausgelesenen Zahlenwerte. Wird hier angelegt um ständiges neu Anlegen zu vermeiden.
	Vector valueVector;		// Speichert Position bzw. Geschwindigkeit beim Auslesen.

	do
	{
		// neue Position für next und current suchen
		current = line.find('=',current + 1);
		next = line.find(';', next + 1);

		// Zur Sicherheit prüfen, ob current und next gescheite Werte enthalten
		if((size_t)current != string::npos && (size_t)next != string::npos)
		{
			// Parameteridentifikationschar auslesen
			// Wert auslesen
			string subString = line.substr(current - 1, paramWidth);
			string subValue = line.substr(current + 1, (next - (current + 1)));


			switch(*subString.c_str())
			{
			case 'p':
				// Position, muß gesondert behandelt werden
				valueVector = vectorFromString(subValue);
				newParticle->setInitialPosition(&valueVector);
				break;
			case 'r':
				// Radius, einfaches parsen
				valuedouble = doubleFromString(subValue);//valueFromString<double>(valuedouble, subValue, std::dec);
				newParticle->setRadius(valuedouble);
			  	break;
			case 'm':
			  	// Masse
				valuedouble = doubleFromString(subValue);//valueFromString<double>(valuedouble, subValue, std::dec);
				newParticle->setMass(valuedouble);
			  	break;
			case 'c':
				// Ladung
				valuedouble = doubleFromString(subValue);//valueFromString<double>(valuedouble, subValue, std::dec);
				newParticle->setCharge(valuedouble);
				break;
			case 'v':
				valueVector = vectorFromString(subValue);
				// Geschwindigkeit, muß gesondert behandelt werden (wie Position)
				newParticle->setVelocity(&valueVector);
				break;
			case 't':
				// Tag
				newParticle->setTag(subValue);
				break;
			}

		}
		// Wenn nicht, dann wird die Schleife am Ende des Durchlaufs abgebrochen
	}
	while((size_t)next != string::npos && (size_t)current != string::npos);

	// transform the particles coordinates
	// TODO: add an option to not transform the coordinates
	newParticle->transformCoordinates();

	return newParticle;
}

/*
 * Transforms the coordinates from the real coordinates to a systen in which the Hamiltonian
 * is diagonalized. The transformation is the same for ALL types of coupling. (e.g. next-
 * neighbor or all with all)
 */
void Particle::transformCoordinates()
{
	ENTER

	//TODO: implement the transformation

	LEAVE
}

Particle* Particle::copy()
{
	Particle* particle = new Particle();

	//Alle Eigenschaften übertragen
	return particle;
}

void Particle::push(Vector* velocity)
{
	velocity_ += *velocity;
}

/*
 * Euler-Version
 *
 * Moves the particle and resets the force. Needs to be called on the first time step
 * since the other algorithms need more input
 */
void Particle::moveEuler(double dt)
{
		//cout << "Bewege Teilchen:" << endl << "Position (alt): " << position_[0] << "," << position_[1] << endl << "Geschwindigkeit (alt): " << velocity_[0] << "," << velocity_[1] << endl;
		if(currentForce_ != Vector(0,0))
			velocity_ += currentForce_ / mass_ * dt;
		// move the particle
		position_ += velocity_*dt;
		// Reset force for the next timestep!
		currentForce_ = Vector(0,0);
		//cout << "Bewege Teilchen:" << endl << "Position (neu): " << position_[0] << "," << position_[1] << endl << "Geschwindigkeit (neu): " << velocity_[0] << "," << velocity_[1] << endl;
}


/*
 * Gives a dump of all properties in a more readable form (each property in it's own line)
 */
string Particle::toString()
{
	ostringstream oss;
	oss << "Particle (" << this->getID() << ")" << endl;
	oss << "Position:\t" << this->getPosition() << endl;
	oss << "Velocity:\t" << this->getVelocity() << endl;
	oss << "Radius:  \t" << this->getRadius() << endl;
	oss << "Mass:    \t" << this->getMass() << endl;
	oss << "Charge:  \t" << this->getCharge() << endl;
	oss << "Tag:     \t" << this->getTag() << endl;
	return oss.str();
}

/*
 * Gives a dump of all properties but in a single line
 */
string Particle::toShortString()
{
	ostringstream oss;
	oss << "Particle (" << this->getID() << ")" << " ";
	oss << "pos: " << (*this->getPosition())[0] << ", " << (*this->getPosition())[1];
#ifdef THREE_DIMENSIONS
	oss << ", " << (*this->getPosition())[2];
#endif
	oss << " ";
	oss << "vel: " << (*this->getVelocity())[0] << ", " << (*this->getVelocity())[1] << " ";
#ifdef THREE_DIMENSIONS
	oss << ", " << (*this->getVelocity())[0];
#endif
	oss << " ";
	oss << "r: " << this->getRadius() << " ";
	oss << "m: " << this->getMass() << " ";
	oss << "c: " << this->getCharge() << " ";
	oss << "t: " << this->getTag() << " ";
	return oss.str();
}

/*
 * Gives a dump of the particles position and velocities as comma seperated values
 */
void Particle::toCSV()
{
	//TODO: gescheit umbauen, bzw. Mathematica anpassen, dass die Klammern nicht ersetzt werden müssen
	ostringstream oss;
	oss << position_[0] << "\t" << position_[1] << "\t";
#ifdef THREE_DIMENSIONS
	oss << position_[2] << "\t";
#endif
	//<< velocity_[0] << "," << velocity_[1] << "\t";
	string s = oss.str();
	cout << s << flush;
	//return oss.str();
}

//
// ----------------- Properties -----------------
//
void Particle::setPosition(Vector* position)
{
	position_[0] = (*position)[0];
	position_[1] = (*position)[1];
}

Vector* Particle::getPosition()
{
	return &position_;
}

void Particle::setInitialPosition(Vector* position)
{
	// 3D Anpassung nötig!
	position_[0] = (*position)[0];
	position_[1] = (*position)[1];

	origin_[0] = (*position)[0];
	origin_[1] = (*position)[1];

#ifdef THREE_DIMENSIONS
	position_[2] = (*position)[2];
	origin_[2] = (*position)[2];
#endif
}

void Particle::setVelocity(Vector* velocity)
{
	velocity_ = *velocity;
}

Vector* Particle::getVelocity()
{
	return &velocity_;
}

Vector* Particle::getCurrentForce()
{
	return &currentForce_;
}

Vector* Particle::getLastForce()
{
	return &lastForce_;
}

void Particle::setCurrentForce(Vector* currentForce)
{
	currentForce_ = *currentForce;
}

void Particle::setLastForce(Vector* force)
{
	lastForce_ = *force;
	/*
	lastForce_[0] = (*force)[0];
	lastForce_[1] = (*force)[1];
#ifdef THREE_DIMENSIONS
	lastForce_[2] = (*force)[2];
#endif
	 */
}

void Particle::resetCurrentForce()
{
	currentForce_ = Vector::Zero();
}

void Particle::switchForce()
{
	lastForce_ = currentForce_;
}

void Particle::addPosition(Vector* translation)
{
	position_ += *translation;
}

void Particle::addVelocity(Vector* acceleration)
{
	velocity_ += *acceleration;
}

void Particle::addForce(Vector* force)
{
	currentForce_ += (*force);
}

void Particle::setRadius(double radius)
{
	radius_ = radius;
}

double Particle::getRadius()
{
	return radius_;
}

void Particle::setCharge(double charge)
{
	charge_ = charge;
}

double Particle::getCharge()
{
	return charge_;
}

double Particle::getMass()
{
	return mass_;
}

void Particle::setMass(double mass)
{
	mass_ = mass;
}


} // end of namespace
