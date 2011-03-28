#pragma once
#include "Enemy.h"

class EnemyCrawler : public Enemy
{
public:
	EnemyCrawler();
	virtual void update(float dt);
	virtual void draw();
	virtual void die();
	//virtual void doDamage(float damage);
	virtual void onAnimationComplete(std::string anim);
private:
	float walk_speed;
}; 
