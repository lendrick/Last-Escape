#ifndef ENEMY_H
#define ENEMY_H

#include "AnimatedActor.h"
#include "Sound.h"

class Enemy : public AnimatedActor
{
public:
	Enemy();
	
	virtual void collide(Actor& otherActor);

	virtual bool isEnemy() { return true; }
	
	Sound* fireSound;
	
	void patrol(float dt);
protected:
	float speed_x, speed_y;

};



#endif
