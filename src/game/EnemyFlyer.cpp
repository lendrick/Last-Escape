#include "EnemyFlyer.h"
#include "Player.h"
#include "Map.h"
#include "SoundCache.h"
#include "Collectible.h"

EnemyFlyer::EnemyFlyer()
:Enemy()
{
	setImage("flyer.png");
	patrolLength = 100;
	
	fly_speed = 200.f;
	
	speed_x = 0;
	speed_y = 0;
	patrolCountdown = patrolLength;
	
	//pick a random death sound
	int sound_num = rand() % 19;
	sound_num += 1;
	std::string s;
	std::stringstream out;
	out << sound_num;
	s = out.str();
	
	std::string sound_file = s + "-BugSplat.ogg";
	//cout << sound_file;
	fireSound = soundCache[sound_file];	
	
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
	
	tmp = addAnimation("die");
	tmp->addFrame(4, .1f);
	tmp->addFrame(5, .1f);
	tmp->addFrame(6, .1f);
	tmp->addFrame(7, .1f);
	
	setCurrentAnimation("fly");	
	//setPlaceholder(sf::Color(255, 0, 0), 16, 32, 0.5f, 1.0f);
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
	if(!dying) move(mx, my);
	
	updateSpriteFacing();
	
	checkCollisions();
}

void EnemyFlyer::die() {
	setCanCollide(false);
	dying = true;
	speed_x = 0;
	speed_y = 0;
	setCurrentAnimation("die");
	fireSound->playSound();
}

void EnemyFlyer::onAnimationComplete(std::string anim) {
	//cout << "EnemyWalker::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		destroy();
		CollectibleEnergyBall * ball = new CollectibleEnergyBall();
		ball->setPos(pos_x, pos_y-30);
	}
}

