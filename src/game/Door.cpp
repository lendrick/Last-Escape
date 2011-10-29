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
	setFrameSize(149, 236);
	setDrawOffset(149/2, 236/2);
//	sprite.SetScale(0.5,0.5);
	sprite.SetScale(1.0/(149.0/w), 1.0/(236.0/h));
	this->name = name;

	Animation * tmp;
	tmp = addAnimation("be a door");
	tmp->addFrame(0, .2f);

	setCurrentAnimation("be a door");


	// Give it some friction
	shape->u = 1.f;
	shape->collision_type = PhysicsType::Door;
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
