#include "PlayerBullet.h"

#include "Enemy.h"
#include "globals.h"

PlayerBullet::PlayerBullet(int facing, float angleVariation):
AnimatedActor()
{
	this->setImage("xeon-bullet.png");
	
	facing_direction = facing;

	float speed = 480.f;

	float angle = ((facing_direction == FACING_RIGHT) ? 90 : -90) + (rand() % 200 - 100) * angleVariation/100;
	speed_x = (int)(0.5f + sin(angle * 3.14159/180.0) * speed);
	speed_y = (int)(0.5f + cos(angle * 3.14159/180.0) * speed);
	
	width = 16;
	height = 8;
	xOrigin = width/2;
	yOrigin = height;
	setDrawOffset(16, 8);
	setFrameSize(16, 16);
	
	Animation * tmp;
	
	tmp = addAnimation("bullet");
	tmp->addFrame(0, .1f);
	tmp->addFrame(1, .1f);
	tmp->addFrame(2, .1f);
	tmp->addFrame(3, .1f);
	tmp->addFrame(4, .1f);
	tmp->setDoLoop(true);
	
	setCurrentAnimation("bullet");
}

void PlayerBullet::collide(Actor& otherActor) {
	if (otherActor.isEnemy())
	{
		otherActor.die();
		die();
	}
}

void PlayerBullet::update(float dt) {
	float mx = speed_x*dt;
	float my = speed_y*dt;
	bool impact = move(mx, my);
	updateSpriteFacing();
	checkCollisions();
	if(impact) destroy();
}
