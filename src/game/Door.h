/*
 * Door.h
 *
 *  Created on: 02.06.2011
 *      Author: Nehmulos
 */

#ifndef DOOR_H_
#define DOOR_H_

#include "AnimatedActor.h"

class Door: public AnimatedActor
{
public:
	Door(double x, double y);
	virtual ~Door();

	void doUpdate(sf::Uint32 dt);

private:
	bool opening;
};

#endif /* DOOR_H_ */
