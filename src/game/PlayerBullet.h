#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include "AnimatedActor.h"

class PlayerBullet  : public AnimatedActor
{
public:
	PlayerBullet(bool rightwards);
	virtual void collide(Actor& otherActor);
	virtual void update(float dt);
private:
	float bullet_speed;
};

#endif
