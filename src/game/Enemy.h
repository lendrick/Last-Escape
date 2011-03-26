#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"

class Enemy : public Actor
{
public:
	virtual void collide(Actor& otherActor);

	virtual bool isEnemy() { return true; }
};

class EnemyWalker : public Enemy
{
public:
	EnemyWalker();
	virtual void update(float dt);
private:
	float walk_speed;
	float speed_x;
	float speed_y;
};

#endif
