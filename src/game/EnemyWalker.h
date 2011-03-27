#pragma once
#include "Enemy.h"

class EnemyWalker : public Enemy
{
public:
	EnemyWalker();
	virtual void update(float dt);
	virtual void draw();
	virtual void die();
	virtual void onAnimationComplete(std::string anim);
private:
	float walk_speed;
}; 
