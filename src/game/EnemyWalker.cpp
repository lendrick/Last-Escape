#include "EnemyWalker.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"

EnemyWalker::EnemyWalker()
:Enemy()
{	
	setImage(*walkerImage);
	walk_speed = 120.f;
	
	speed_x = 0;
	speed_y = 0;
	
	width = 24;
	height = 20;
	xOrigin = width/2;
	yOrigin = height;
	setDrawOffset(16, 32);
	setFrameSize(32, 32);
	
	Animation * tmp;
	
	tmp = addAnimation("walk");
	tmp->addFrame(0, .2f);
	tmp->addFrame(1, .2f);
	tmp->addFrame(2, .2f);
	tmp->addFrame(1, .2f);
	tmp->setDoLoop(true);
	
	setCurrentAnimation("walk");
}

EnemyWalker::~EnemyWalker()
{
}

void EnemyWalker::update(float dt) {
	
	const int speed_gravity = 960;
	const float vision_range = 320;
	
	speed_y += speed_gravity*dt;
	if(isGrounded()) speed_y = 0;
	
	// Chase the player
	float dx = g_player->pos_x - pos_x;
	if (-vision_range < dx && dx < 0) {
		speed_x = -walk_speed;
		facing_direction = FACING_RIGHT;
	} else if (0 < dx && dx < vision_range) {
		speed_x = walk_speed;
		facing_direction = FACING_LEFT;
	}
	updateSpriteFacing();
	
	float delta_x = speed_x*dt;
	float delta_y = speed_y*dt;
	move(delta_x, delta_y);
	speed_x = delta_x / dt;
	speed_y = delta_y / dt;
	
	checkCollisions();
}

