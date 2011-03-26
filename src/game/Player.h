#ifndef PLAYER_H
#define PLAYER_H

#include "Map.h"
#include "Actor.h"
#include "AnimatedActor.h"
#include "globals.h"

class Player : public AnimatedActor {
public:
	Player();
	~Player();

	virtual bool isPlayer() { return true; }

	virtual void update(float dt);
	virtual void draw();
	virtual void die();
	virtual void collide(Actor & otherActor);
	void init();

	void jump();
	void shoot();

	float last_shoot_time;

	float energy;
	float energy_max;

	bool facing_rightwards;
	float speed_x;
	float speed_y;
};


#endif
