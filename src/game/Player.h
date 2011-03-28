#ifndef PLAYER_H
#define PLAYER_H

#include "Map.h"
#include "AnimatedActor.h"
#include "Sound.h"
#include "globals.h"
#include "StartPoint.h"

class Player : public AnimatedActor {
public:
	Player();

	virtual bool isPlayer() { return true; }

	virtual void update(float dt);
	virtual void draw();
	virtual void die();
	virtual void collide(Actor & otherActor);
	virtual void onDestroy();
	virtual void onAnimationComplete(std::string anim);
        virtual void doDamage(float damage);
        virtual void onDamage();
	
	StartPoint * findStart();
	
	StartPoint * currentStart;
	void init();

	void jump(float dt);
	void shoot();
	void crouch();

	void upgradeWeapon();

	int current_weapon;

	float time;
	float last_shoot_time;
	float last_jump_time;

	float energy;
	float energy_max;

	float speed_x;
	float speed_y;
	
	float shoot_duration;
	
	bool crouched;
	bool walking;
	
	int armor;
	int lifes;
	int energyBalls;
        float recoveryTime;
        float recoveryTimer;
	
	Sound * fireSound;
	Sound * jumpSound;
	Sound * dieSound;
};


#endif
