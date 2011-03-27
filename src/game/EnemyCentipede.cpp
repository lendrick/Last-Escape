#include "EnemyCentipede.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"
#include "Collectible.h"

EnemyCentipede::EnemyCentipede()
:Enemy()
{	
	setImage("centipede.png");
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
	tmp->addFrame(2, .2f);
	tmp->addFrame(3, .2f);
	tmp->setDoLoop(true);
	
	tmp = addAnimation("die");
	tmp->addFrame(0, .07f);
	tmp->addFrame(4, .07f);
	tmp->addFrame(8, .07f);
	tmp->addFrame(12, .07f);
	
	tmp = addAnimation("shoot");
	tmp->addFrame(11, .1f);
	tmp->addFrame(10, .1f);
	tmp->addFrame(9, .1f);
	
	setCurrentAnimation("walk");
}

EnemyCentipede::~EnemyCentipede()
{
}

void EnemyCentipede::update(float dt) {
	if(!dying) {		
		float mx, my;
		if(facing_direction == FACING_LEFT) {
			mx = -1;
			my = 0;
		} else if(facing_direction == FACING_RIGHT) {
			mx = 1;
			my = 0;
		}
		
		int check_pos = 12;
		if(facing_direction == FACING_LEFT) {
			check_pos = -12;
		}
		
		if(!move(mx, my)) {
			// Turn around if you run into something.
			flipDirection();
		} else if(!game_map->isSolid(pos_x + check_pos, pos_y + 1)) {
			// Turn around if there's a pit up ahead.
			flipDirection();
		}
		
		updateSpriteFacing();
		
		checkCollisions();
	}
}

void EnemyCentipede::draw() {
	//cout << "walker frame " << currentAnimation->getFrame() << "\n";
	AnimatedActor::draw();
}

void EnemyCentipede::die() {
	setCanCollide(false);
	dying = true;
	speed_x = 0;
	speed_y = 0;
	setCurrentAnimation("die");
}

void EnemyCentipede::onAnimationComplete(std::string anim) {
	//cout << "EnemyCentipede::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		destroy();
		CollectibleEnergyBall * ball = new CollectibleEnergyBall();
		ball->setPos(pos_x, pos_y-16);
	} else if(anim == "shoot") {
		//shoot a projectile
	}
}

