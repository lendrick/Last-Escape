#include "EnemyFlyer.h"
#include "Player.h"
#include "Map.h"

EnemyFlyer::EnemyFlyer()
:Enemy()
{
	setImage(*flyerImage);
	patrolLength = 100;
	
	fly_speed = 200.f;
	
	speed_x = 0;
	speed_y = 0;
	patrolCountdown = patrolLength;
	
	facing_direction = FACING_LEFT;
	width = 24;
	height = 32;
	xOrigin = width/2;
	yOrigin = height;
	setDrawOffset(12, 64);
	setFrameSize(32, 64);
	
	Animation * tmp;
	tmp = addAnimation("fly");
	tmp->addFrame(0, .2f);
	tmp->addFrame(1, .2f);
	tmp->addFrame(2, .2f);
	tmp->setDoLoop(true);
	
	setCurrentAnimation("fly");	
	//setPlaceholder(sf::Color(255, 0, 0), 16, 32, 0.5f, 1.0f);
}

EnemyFlyer::~EnemyFlyer()
{
}

void EnemyFlyer::update(float dt) {
	const float vision_range = 320;
	
	patrolCountdown--;
	
	if(patrolCountdown == 0) {
		flipDirection();
		patrolCountdown = patrolLength;
	}
	
	float mx, my;
	if(facing_direction == FACING_LEFT) {
		mx = -1;
		my = 0;
	} else if(facing_direction == FACING_RIGHT) {
		mx = 1;
		my = 0;
	}
	move(mx, my);
	
	updateSpriteFacing();
	
	checkCollisions();
}


