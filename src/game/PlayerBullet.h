#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include "Actor.h"

class PlayerBullet  : public Actor
{
public:
	PlayerBullet(bool rightwards);
	virtual void collide(Actor& otherActor);
	virtual void update(float dt);
private:
	float bullet_speed;
};

#endif
