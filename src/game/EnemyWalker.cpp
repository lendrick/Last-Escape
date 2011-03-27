#include "EnemyWalker.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"
#include "Collectible.h"

EnemyWalker::EnemyWalker()
:Enemy()
{	
	setImage("walker.png");
	walk_speed = 120.f;
	
	speed_x = 0;
	speed_y = 0;
	dying = false;
	
	width = 24;
	height = 20;
	xOrigin = width/2;
	yOrigin = height;
	setDrawOffset(16, 30);
	setFrameSize(32, 32);
	
	Animation * tmp;
	
	tmp = addAnimation("walk");
	tmp->addFrame(3, .2f);
	tmp->addFrame(2, .2f);
	tmp->addFrame(1, .2f);
	tmp->addFrame(2, .2f);
	tmp->setDoLoop(true);
	
	tmp = addAnimation("die");
	tmp->addFrame(7, .07f);
	tmp->addFrame(6, .07f);
	tmp->addFrame(5, .07f);
	tmp->addFrame(4, .07f);
	
	setCurrentAnimation("walk");
}

EnemyWalker::~EnemyWalker()
{
}

void EnemyWalker::update(float dt) {
	if(!dying) {
		const int speed_gravity = 960;
		const float vision_range = 320;
		const float vision_min_range = 32;
		
		speed_y += speed_gravity*dt;
		if(isGrounded()) speed_y = 0;
		
		/*
		// Chase the player
		float dx = g_player->pos_x - pos_x;
		if (-vision_range < dx && dx < -vision_min_range) {
			speed_x = -walk_speed;
			facing_direction = FACING_RIGHT;
		} else if (vision_min_range < dx && dx < vision_range) {
			speed_x = walk_speed;
			facing_direction = FACING_LEFT;
		}
		*/
		if(facing_direction == FACING_LEFT) {
			speed_x = -60.0f;
		} else {
			speed_x = 60.0f;
		}
		patrol(dt);
		
		updateSpriteFacing();
	
		checkCollisions();
	}
}

void EnemyWalker::draw() {
	//cout << "walker frame " << currentAnimation->getFrame() << "\n";
	AnimatedActor::draw();
}

void EnemyWalker::die() {
	setCanCollide(false);
	dying = true;
	speed_x = 0;
	speed_y = 0;
	setCurrentAnimation("die");
}

void EnemyWalker::onAnimationComplete(std::string anim) {
	//cout << "EnemyWalker::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		destroy();
		CollectibleEnergyBall * ball = new CollectibleEnergyBall();
		ball->setPos(pos_x, pos_y-16);
	}
}
