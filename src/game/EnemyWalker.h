#pragma once
#include "Enemy.h"

class EnemyWalker : public Enemy
{
public:
	EnemyWalker();
	~EnemyWalker();
	virtual void update(float dt);
private:
	float walk_speed;
	float speed_x;
	float speed_y;
}; 
