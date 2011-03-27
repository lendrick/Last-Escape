#ifndef ENEMY_H
#define ENEMY_H

#include "AnimatedActor.h"
#include "Sound.h"
#include "SoundCache.h"

class Enemy : public AnimatedActor
{
public:
	Enemy();
	~Enemy();
	
	virtual void collide(Actor& otherActor);

	virtual bool isEnemy() { return true; }
	
	Sound* fireSound;
	SoundCache soundCache;
	
	void patrol(float dt);
protected:
	float speed_x, speed_y;

};



#endif
