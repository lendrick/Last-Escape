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

#include "EnemyFlyer.h"
#include "Player.h"
#include "Map.h"
#include "SoundCache.h"
#include "Collectible.h"

EnemyFlyer::EnemyFlyer(float x, float y)
:Enemy(x, y, 27.0f, 31.0f)
{
	setImage("flyer.png");
	patrolLength = 100;

	fly_speed = 200.f;

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

	facing_direction = Facing::Left;
	setDrawOffset(15, 52);
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
	
	x1 = body->p.x - 100;
	x2 = body->p.x + 100;
	y1 = body->p.y - 20;
	y2 = body->p.y + 40;
	
	shape->u = 0.1f;
	
  timeUntilDive = 0;
	timeUntilEndDive = 0;
	
	minDiveTime = 0.1f;
	maxDiveTime = 0.3f;
	minTimeBetweenDives = .8f;
	maxTimeBetweenDives = 2.5f;

	diving = false;
	
	setCurrentAnimation("fly");
	//setPlaceholder(sf::Color(255, 0, 0), 16, 32, 0.5f, 1.0f);
}


void EnemyFlyer::update(float dt) {
	cpVect force = cpv(0, 0);
	
  if(diving && timeUntilEndDive <= 0) {
		diving = false;
		timeUntilDive = frand(minTimeBetweenDives, minTimeBetweenDives);
	} else if(!diving && timeUntilDive <= 0) {
		diving = true;
		timeUntilEndDive = frand(minDiveTime, maxDiveTime);
	}
	
	if(facing_direction == Facing::Left) {
	  if(body->p.x < x1) {
			facing_direction = Facing::Right;
		} else if(body->v.x > -fly_speed) {
			force.x = -75;
		}
	} else if(facing_direction == Facing::Right) {
	  if(body->p.x > x2) {
			facing_direction = Facing::Left;
		} else if(body->v.x < fly_speed) {
			force.x = 75;
		}
	}
	
	
	// Doesn't matter if they go below zero.
	timeUntilEndDive -= dt;
	timeUntilDive -= dt;

	if(!diving && body->p.y < y1 && body->v.y < fly_speed) {
		force.y = 500;
	}
	
	cpBodyApplyImpulse(body, force, cpv(0, 0));
	
	updateSpriteFacing();

	//checkcollisions();
}

void EnemyFlyer::die() {
	setCanCollide(false);
	dying = true;
	freeze();
	setCurrentAnimation("die");
	fireSound->playSound();
}

void EnemyFlyer::onAnimationComplete(std::string anim) {
	//cout << "EnemyWalker::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		destroy();
		CollectibleEnergyBall * ball = new CollectibleEnergyBall(body->p.x, body->p.y+8);
	}
}

