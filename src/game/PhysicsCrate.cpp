#include "PhysicsCrate.h"

PhysicsCrate::PhysicsCrate(double x, double y)
:AnimatedActor(x, y, 30.0, 30.0)
{
	actorName = "Crate";
	setImage("crate.png");
	setFrameSize(32, 32);
  setDrawOffset(17, 17);
	
	Animation * tmp;
	tmp = addAnimation("be a crate");
	tmp->addFrame(0, .2f);
	
	setCurrentAnimation("be a crate");
	
	// Give it some friction
	shape->u = 0.7f;
	
	// Change the rotational mass to a finite number so the crate
	// can rotate.
	cpBodySetMass(body, 30.0);
	//cout << "crate mass: " << body->m << "\n";
	cpBodySetMoment(body, 7000.0);
	shape->collision_type = PhysicsType::Crate;
	shape->e = 0.0;

	life = 4;
}

PhysicsCrate::~PhysicsCrate()
{
}
