#pragma once
#include "Enemy.h"

class EnemyFlyer : public Enemy
{
public:
	EnemyFlyer();
	virtual void update(float dt);
	virtual void die();
	virtual void onAnimationComplete(std::string anim);
private:
	int patrolCountdown, patrolLength;
	float fly_speed;
}; 
