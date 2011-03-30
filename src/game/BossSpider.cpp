/*
 *  This file is part of Last Escape.
 *
 *  Last Escape is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Last Escape is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Last Escape.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "BossSpider.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"
#include "Collectible.h"
#include "Sound.h"
#include "SoundCache.h"
#include "EnemyCentipede.h"


BossSpider::BossSpider(float x, float y)
:Enemy(x, y)
{
	setImage("spider.png");
	walk_speed = 100.0f;

	speed_x = 0;
	speed_y = 0;
	dying = false;
	life = 5;

	width = 56;
	height = 41;
	xOrigin = width/2;
	yOrigin = height;
	setDrawOffset(32, 63);
	setFrameSize(64, 64);
	lastShot = 0;
	shootInterval = 0.5f;
	time = 0;

	patrolInterval = 1.5f;
	patrolTime = 0;

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
	tmp->addFrame(8, .2f);
	tmp->addFrame(9, .2f);
	tmp->addFrame(10, .2f);
	tmp->addFrame(11, .2f);

	tmp = addAnimation("hurt");
	tmp->addFrame(0, 0.07f);

	setCurrentAnimation("walk");
}

void BossSpider::update(float dt) {
	if(!dying) {
		time += dt;
		patrolTime += dt;

		//setCurrentAnimation("walk");
		const int speed_gravity = 960;
		const float vision_range = 320;
		const float vision_min_range = 32;

		speed_y += speed_gravity*dt;
		if(isGrounded()) speed_y = 0;

		if(facing_direction == FACING_LEFT) {
			speed_x = -walk_speed * dt;
			if(patrolTime > patrolInterval) {
				facing_direction = FACING_RIGHT;
				patrolTime = 0;
			}
		} else {
			speed_x = walk_speed * dt;
			if(patrolTime > patrolInterval) {
				facing_direction = FACING_LEFT;
				patrolTime = 0;
			}
		}

		move(speed_x, speed_y);

		updateSpriteFacing();

		if(lastShot + shootInterval < time) {
			lastShot = time;
			EnemyCentipedeProjectile * projectile =
				new EnemyCentipedeProjectile(facing_direction, (int)pos_x, int(pos_y - 20.0f));
		}

		checkCollisions();
	}
}

void BossSpider::draw() {
	//cout << "walker frame " << currentAnimation->getFrame() << "\n";
	AnimatedActor::draw();
}

bool BossSpider::doDamage(float damage) {
	life -= damage;
	if(life <= 0) {
		die();
		return true;	
	} else {
		setCurrentAnimation("hurt");
	}

	return false;
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
		CollectibleEnergyBall * ball = NULL;
		ball = new CollectibleEnergyBall(pos_x-32, pos_y-16);
		ball = new CollectibleEnergyBall(pos_x, pos_y-16);
		ball = new CollectibleEnergyBall(pos_x-32, pos_y-48);
		ball = new CollectibleEnergyBall(pos_x, pos_y-48);
	}

	if(anim == "hurt") {
		setCurrentAnimation("walk");
	}
}


