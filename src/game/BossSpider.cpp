#include "BossSpider.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"
#include "Collectible.h"
#include "Sound.h"
#include "SoundCache.h"
#include "EnemyCentipede.h"


BossSpider::BossSpider()
:Enemy()
{	
	setImage("spider.png");
	walk_speed = 1.f;
	
	speed_x = 0;
	speed_y = 0;
	dying = false;
	life = 5;
	
	width = 60;
	height = 40;
	xOrigin = width/2;
	yOrigin = height;
	setDrawOffset(45, 40);
	setFrameSize(64, 64);
	lastShot = 0;
	shootInterval = 0.5f;
	time = 0;
	
	Animation * tmp;
	
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
	
	
	tmp = addAnimation("walk");
	tmp->addFrame(0, .2f);
	tmp->addFrame(1, .2f);
	tmp->addFrame(2, .2f);
	tmp->addFrame(4, .2f);
	tmp->addFrame(5, .2f);
	tmp->setDoLoop(true);
	
	tmp = addAnimation("die");
	tmp->addFrame(0, .07f);
	
	tmp = addAnimation("hurt");
	tmp->addFrame(0, 0.07f);
	
	setCurrentAnimation("walk");
}

void BossSpider::update(float dt) {
	if(!dying) {
		time += dt;
		
		//setCurrentAnimation("walk");
		const int speed_gravity = 960;
		const float vision_range = 320;
		const float vision_min_range = 32;
		
		speed_y += speed_gravity*dt;
		if(isGrounded()) speed_y = 0;
		
		if(facing_direction == FACING_LEFT) {
			speed_x = -walk_speed;
		} else {
			speed_x = walk_speed;
		}
		patrol(dt);
		
		updateSpriteFacing();
		
		if(lastShot + shootInterval < time) {
			lastShot = time;
			EnemyCentipedeProjectile * projectile = 
				new EnemyCentipedeProjectile(facing_direction, pos_x, pos_y - 20);
		}
		
		checkCollisions();
	}
}

void BossSpider::draw() {
	//cout << "walker frame " << currentAnimation->getFrame() << "\n";
	AnimatedActor::draw();
}

void BossSpider::doDamage(float damage) {
	life -= damage;
	if(life <= 0) 
		die();
	else 
		setCurrentAnimation("hurt");
	
}

void BossSpider::die() {
	setCanCollide(false);
	dying = true;
	speed_x = 0;
	speed_y = 0;
	setCurrentAnimation("die");
	fireSound->playSound();
}

void BossSpider::onAnimationComplete(std::string anim) {
	//cout << "BossSpider::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		destroy();
		CollectibleEnergyBall * ball = new CollectibleEnergyBall();
		ball->setPos(pos_x-16, pos_y-16);
	}
	
	if(anim == "hurt") {
		setCurrentAnimation("walk");
	}
}


