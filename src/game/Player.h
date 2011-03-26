#ifndef PLAYER_H
#define PLAYER_H

#include "Map.h"
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

	int current_weapon;

	float anim_time;
	float last_shoot_time;

	float energy;
	float energy_max;

	float speed_x;
	float speed_y;
};


#endif
