/*
 * Door.cpp
 *
 *  Created on: 02.06.2011
 *      Author: Nehmulos
 */

#include "Door.h"

Door::Door(double x, double y)
: AnimatedActor(x, y, 32.0, 64.0, "door.png", true)
{
	actorName = "Door";
	setFrameSize(32, 64);
	setDrawOffset(17, 17);

	Animation * tmp;
	tmp = addAnimation("be a door");
	tmp->addFrame(0, .2f);

	setCurrentAnimation("be a door");


	// Give it some friction
	shape->u = 1.f;
	// Change the rotational mass to a finite number so the crate
	// can rotate.
//	cpBodySetMass(body, 1.f);
	//cout << "crate mass: " << body->m << "\n";
//	cpBodySetMoment(body, 7000.0);
	shape->collision_type = PhysicsType::Ground;
	shape->e = 0.0;

	opening = true;
}

void Door::doUpdate(sf::Uint32 dt)
{
//	if(opening) {
//		double tx, ty;
//		getPos(tx, ty);
//		cpBodySetPos(body, cpv(tx-1, ty-1));
//	}
}

Door::~Door()
{
	// TODO Auto-generated destructor stub
}
