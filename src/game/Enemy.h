#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"

class Enemy : public Actor
{
public:
	virtual void collide(Actor& otherActor);

	virtual bool isEnemy() { return true; }
};



#endif
