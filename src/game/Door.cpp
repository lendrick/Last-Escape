/*
 * Door.cpp
 *
 *  Created on: 02.06.2011
 *      Author: Nehmulos
 */

#include "Door.h"

Door::Door(double x, double y, double w, double h, std::string name)
: AnimatedActor(x, y, w, h, "door.png", true)
{
	actorName = "Door";
	setFrameSize(w, h);
	setDrawOffset(w/2, h/2);
	this->name = name;

	Animation * tmp;
	tmp = addAnimation("be a door");
	tmp->addFrame(0, .2f);

	setCurrentAnimation("be a door");


	// Give it some friction
	shape->u = 1.f;
	shape->collision_type = PhysicsType::Enemy;
	shape->e = 0.0;
}

void Door::doUpdate(sf::Uint32 dt)
{
	//TODO add an opening animation that shifts the door upwards
}

void Door::collide(Actor& otherActor)
{
	if(otherActor.isPlayer())
	{
		Player& player = (Player&)otherActor;
		if(!player.dying && player.hasKeycard(name))
		{
			destroy();
			soundCache["door_open.oga"]->playSound();
			player.removeKeycard(name);
		}
	}
}

Door::~Door()
{
	// TODO Auto-generated destructor stub
}
