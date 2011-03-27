#include "EnemyCentipede.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"
#include "Collectible.h"
#include "ImageCache.h"
#include <SFML/Graphics.hpp>

EnemyCentipede::EnemyCentipede()
:Enemy()
{	
	//debugPixel.SetImage(*imageCache["bluepixel.png"]);
	setImage("centipede.png");
	walk_speed = 120.f;
	
	speed_x = 0;
	speed_y = 0;
	dying = false;
	
	width = 32;
	height = 25;
	xOrigin = width/2;
	yOrigin = height;
	setDrawOffset(32, 30);
	setFrameSize(64, 32);
	
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
	
	checkGround = false;
}

EnemyCentipede::~EnemyCentipede()
{
}

void EnemyCentipede::update(float dt) {
	//checkGround = false;
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
		
		if(isGrounded()) {
			speed_y = 0;
			if(move(mx, my)) {
				// Turn around if you run into something.
				//cout << "obstructed\n";
				flipDirection();
			} else if(!game_map->isSolid(pos_x + check_pos, pos_y + yOrigin + 1)) {
				// Turn around if there's a pit up ahead.
				//cout << "pit\n";
				//debugPixel.SetPosition(
				//	0.5f + (int)(pos_x - game_map->cam_x),
				//											 0.5f + (int)(pos_y - game_map->cam_y));
				//checkGround = true;
				flipDirection();
			}
		} else {
			const int speed_gravity = 960;
			speed_y += speed_gravity*dt;
			
			float delta_x = speed_x*dt;
			float delta_y = speed_y*dt;
			move(delta_x, delta_y);
		}
		
		updateSpriteFacing();
		
		checkCollisions();
	}
}

void EnemyCentipede::draw() {
	//cout << "centipede frame " << currentAnimation->getFrame() << "\n";
	AnimatedActor::draw();

	//if(checkGround) App->Draw(debugPixel);
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

