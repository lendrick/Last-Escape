#pragma once
#include "Enemy.h"

class BossSpider : public Enemy
{
public:
	BossSpider();
	virtual void update(float dt);
	virtual void draw();
	virtual void die();
	virtual void doDamage(float damage);
	virtual void onAnimationComplete(std::string anim);
	
private:
	float walk_speed;
	float shootInterval;
	float lastShot, time;
	float patrolInterval, patrolTime;
}; 

