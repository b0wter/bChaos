/*
 * Interactions.h
 *
 *  Created on: 19 Feb 2011
 *      Author: b0wter
 */

#ifndef INTERACTIONS_H_
#define INTERACTIONS_H_

/*!
 * Base class for the other two interaction classes. This is not really needed.
 * The Constructor is protected becuase the user should not be able to create any
 * Interactions instance.
 */


namespace Interactions
{

class Interactions
{
public:
	virtual ~Interactions();

	// there are no compute(...) or computePotential(...) methods here since they have different signatures (one-body, two-body)

protected:
	Interactions();

private:
};

}

#endif /* INTERACTIONS_H_ */
