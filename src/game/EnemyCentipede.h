#pragma once
#include "Enemy.h" 

class EnemyCentipede : public Enemy
{
public:
	EnemyCentipede();
	~EnemyCentipede();
	virtual void update(float dt);
	virtual void draw();
	virtual void die();
	virtual void onAnimationComplete(std::string anim);
private:
	float walk_speed;
	float speed_x;
	float speed_y;
}; 

