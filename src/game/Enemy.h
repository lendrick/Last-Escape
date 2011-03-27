#ifndef ENEMY_H
#define ENEMY_H

#include "AnimatedActor.h"

class Enemy : public AnimatedActor
{
public:
	Enemy();
	~Enemy();
	
	virtual void collide(Actor& otherActor);

	virtual bool isEnemy() { return true; }
	
	void patrol(float dt);
protected:
	float speed_x, speed_y;
};



#endif
