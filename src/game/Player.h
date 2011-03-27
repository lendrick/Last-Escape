#ifndef PLAYER_H
#define PLAYER_H

#include "Map.h"
#include "AnimatedActor.h"
#include "Sound.h"
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

	void jump(float dt);
	void shoot();

	int current_weapon;

	float time;
	float last_shoot_time;
	float last_jump_time;

	float energy;
	float energy_max;

	float speed_x;
	float speed_y;
	
	float shoot_duration;
	
	int armor;
	Sound * fireSound;
};


#endif
