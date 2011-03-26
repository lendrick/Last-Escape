#ifndef PLAYER_H
#define PLAYER_H

#include "Map.h"
#include "Actor.h"
#include "globals.h"

class Player : public Actor {
public:
	Player();
	~Player();

	virtual bool isPlayer() { return true; }

	virtual void update(float dt);
	virtual void draw();

	void jump();
	void shoot();

	float anim_time;
	float last_shoot_time;

	float energy;
	float energy_max;

	bool facing_rightwards;
	float speed_x;
	float speed_y;
};


#endif
