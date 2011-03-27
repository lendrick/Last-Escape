#pragma once
#include "Enemy.h" 
#include <SFML/Graphics.hpp>

class EnemyCentipede : public Enemy
{
public:
	EnemyCentipede();
	virtual void update(float dt);
	virtual void draw();
	virtual void die();
	virtual void onAnimationComplete(std::string anim);
	
private:
	float shootInterval;
	float lastShot;
	float time;
	float walk_speed;
	float speed_x;
	float speed_y;
	bool checkGround;
	//sf::Sprite debugPixel;
}; 

class EnemyCentipedeProjectile : public Enemy
{
public:
	EnemyCentipedeProjectile(int direction, int start_x, int start_y);
	virtual void update(float dt);
	virtual void onAnimationComplete(std::string anim);
private:

	float fly_speed;
};