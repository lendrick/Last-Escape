#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include "AnimatedActor.h"

class PlayerBullet  : public AnimatedActor
{
public:
	PlayerBullet(int facing, float angleVariation);
	virtual void collide(Actor& otherActor);
	virtual void update(float dt);
private:
	float speed_x;
	float speed_y;
};

#endif
