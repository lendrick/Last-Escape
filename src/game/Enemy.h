#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"

class Enemy : public Actor
{
public:
	virtual void collidePlayer(Player& player);
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
