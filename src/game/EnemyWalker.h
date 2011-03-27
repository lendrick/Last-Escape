#pragma once
#include "Enemy.h"

class EnemyWalker : public Enemy
{
public:
	EnemyWalker();
	~EnemyWalker();
	virtual void update(float dt);
	virtual void draw();
	virtual void die();
	virtual void onAnimationComplete(std::string anim);
private:
	float walk_speed;
	float speed_x;
	float speed_y;
}; 
