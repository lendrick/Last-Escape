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

#include "EnemyCrawler.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"
#include "Collectible.h"
#include "Sound.h"
#include "SoundCache.h"
#include "Utils.h"
#include "Bumper.h"


EnemyCrawler::EnemyCrawler(double x, double y)
:EnemyPatroller(x, y, 40.0f, 16.0f)
{
	setImage("crawler.png");
	walk_speed = 70.f;
	shape->u = 0.1f;

	dying = false;
	life = 2;

	setDrawOffset(33, 10);
	setFrameSize(64, 32);

	Animation * tmp;
	actorName = "Crawler";
	
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
	facing_direction = Facing::Left;

	tmp = addAnimation("walk");
	tmp->addFrame(2, .2f);
	tmp->addFrame(3, .2f);
	tmp->setDoLoop(true);

	tmp = addAnimation("die");
	tmp->addFrame(8, .07f);
	tmp->addFrame(7, .07f);
	tmp->addFrame(6, .07f);
	tmp->addFrame(5, .07f);

	tmp = addAnimation("hurt");
	tmp->addFrame(4, 0.07f);

	setCurrentAnimation("walk");
}

EnemyCrawler::~EnemyCrawler() {
}


/*
EnemyCrawler::~EnemyCrawler() {
	//cout << "delete crawler " << actorName << "\n";
	delete leftBumper;
	delete rightBumper;
}

void EnemyCrawler::update(double dt) {
	if(!dying) {

		//setCurrentAnimation("walk");
		const int speed_gravity = 960;
		const double vision_range = 320;
		const double vision_min_range = 32;

		if(isGrounded()) {
			if (facing_direction == Facing::Left) {
				if(body->v.x > -walk_speed)
					cpBodyApplyImpulse(body, cpv(-500, 0), cpv(0, 0));
				
				if(!leftBumper->isGrounded())
					facing_direction = Facing::Right;
			} else if (facing_direction  == Facing::Right) {
				if(body->v.x < walk_speed)
					cpBodyApplyImpulse(body, cpv(500, 0), cpv(0, 0));
				
				if(!rightBumper->isGrounded())
					facing_direction = Facing::Left;
			}
		}
		
		updateSpriteFacing();
	}
}

void EnemyCrawler::draw() {
	//cout << "walker frame " << currentAnimation->getFrame() << "\n";
	AnimatedActor::draw();
}

void EnemyCrawler::die() {
	setCanCollide(false);
	dying = true;
	freeze();
	setCurrentAnimation("die");
	fireSound->playSound();
}

void EnemyCrawler::onAnimationComplete(std::string anim) {
	//cout << "EnemyCrawler::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		destroy();
		CollectibleEnergyBall * ball = new CollectibleEnergyBall(pos_x-16, pos_y-16);
	}

	if(anim == "hurt") {
		setCurrentAnimation("walk");
	}
}

void EnemyCrawler::onBumperCollide(int facing) {
	//cout << "Bumper collide " << actorName << "\n";
	if(facing == Facing::Left) {
		facing_direction = Facing::Right;
	} else if(facing == Facing::Right) {
		facing_direction = Facing::Left;
	}
}
*/
