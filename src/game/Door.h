/*
 * Door.h
 *
 *  Created on: 02.06.2011
 *      Author: Nehmulos
 */

#ifndef DOOR_H_
#define DOOR_H_

#include "SoundCache.h"
#include "AnimatedActor.h"
#include "Player.h"

class Door: public AnimatedActor
{
public:
	Door(double x, double y, double w, double h, std::string name);
	virtual ~Door();

	void doUpdate(sf::Uint32 dt);
	virtual void collide(Actor&);
private:
	/// a Keycard with the same name is required to open the door
	std::string name;
};

#endif /* DOOR_H_ */
