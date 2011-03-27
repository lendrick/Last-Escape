#pragma once
#include "Enemy.h"

class EnemyFlyer : public Enemy
{
public:
	EnemyFlyer();
	virtual void update(float dt);
	virtual void die();
private:
	int patrolCountdown, patrolLength;
	float fly_speed;
}; 
