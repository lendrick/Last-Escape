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


BossSpider::BossSpider(double x, double y)
:Enemy(x, y, 56.0f, 41.0f)
{
	setImage("spider.png");
	walk_speed = 100.0f;

	dying = false;
	life = 5;

	setDrawOffset(32, 22+16);
	setFrameSize(64, 64);
	lastShot = 0;
	shootInterval = 500;
	time = 0;

	patrolInterval = 1500;
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
	shape->u = 0.1f;

	tmp = addAnimation("walk");
	tmp->addFrame(0, sf::Milliseconds(200));
	tmp->addFrame(1, sf::Milliseconds(200));
	tmp->addFrame(2, sf::Milliseconds(200));
	tmp->addFrame(4, sf::Milliseconds(200));
	tmp->addFrame(5, sf::Milliseconds(200));
	tmp->setDoLoop(true);

	tmp = addAnimation("die");
	tmp->addFrame(8,  sf::Milliseconds(200));
	tmp->addFrame(9,  sf::Milliseconds(200));
	tmp->addFrame(10, sf::Milliseconds(200));
	tmp->addFrame(11, sf::Milliseconds(200));

	tmp = addAnimation("hurt");
	tmp->addFrame(0, sf::Milliseconds(70));

	setCurrentAnimation("walk");
}

void BossSpider::update(sf::Uint32 dt) {
	if(!dying) {
		time += dt;
		patrolTime += dt;

		//setCurrentAnimation("walk");
		const int speed_gravity = 960;
		const double vision_range = 320;
		const double vision_min_range = 32;

		if(facing_direction == Facing::Left) {
			if(body->v.x > -walk_speed)
				cpBodyApplyImpulse(body, cpv(-500, 0), cpv(0, 0));
			
			if(patrolTime > patrolInterval) {
				facing_direction = Facing::Right;
				patrolTime = 0;
			}
		} else {
			if(body->v.x < walk_speed)
					cpBodyApplyImpulse(body, cpv(500, 0), cpv(0, 0));
			
			if(patrolTime > patrolInterval) {
				facing_direction = Facing::Left;
				patrolTime = 0;
			}
		}

		updateSpriteFacing();

		if(lastShot + shootInterval < time) {
			lastShot = time;
			EnemyCentipedeProjectile * projectile =
				new EnemyCentipedeProjectile(facing_direction, body->p.x, int(body->p.y + 20.0f));
		}

		//checkcollisions();
	}
}

void BossSpider::draw() {
	//cout << "walker frame " << currentAnimation->getFrame() << "\n";
	AnimatedActor::draw();
}

bool BossSpider::doDamage(double damage) {
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
	freeze();
	setCurrentAnimation("die");
	fireSound->playSound();
}

void BossSpider::onAnimationComplete(std::string anim) {
	//cout << "BossSpider::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		double pos_x = body->p.x;
		double pos_y = body->p.y;
		destroy();
		CollectibleEnergyBall * ball = NULL;
		ball = new CollectibleEnergyBall(pos_x-16, pos_y-10);
		ball = new CollectibleEnergyBall(pos_x+16, pos_y-10);
		ball = new CollectibleEnergyBall(pos_x-16, pos_y+22);
		ball = new CollectibleEnergyBall(pos_x+16, pos_y+22);
	}

	if(anim == "hurt") {
		setCurrentAnimation("walk");
	}
}


