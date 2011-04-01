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

#include "EnemyPatroller.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"
#include "Collectible.h"
#include "Sound.h"
#include "SoundCache.h"
#include "Utils.h"
#include "Bumper.h"


EnemyPatroller::EnemyPatroller(float x, float y, float w, float h)
:Enemy(x, y, w, h)
{
	walk_speed = 0;
	shape->u = 0.1f;

	dying = false;
	leftBumper = rightBumper = NULL;
	facing_direction = Facing::Left;
	leftBumper = new Bumper(this, Facing::Left);
	rightBumper = new Bumper(this, Facing::Right);
}

EnemyPatroller::~EnemyPatroller() {
	//cout << "delete patroller " << actorName << "\n";
	delete leftBumper;
	delete rightBumper;
}

void EnemyPatroller::update(float dt) {
	if(!dying) {

		//setCurrentAnimation("walk");
		const int speed_gravity = 960;
		const float vision_range = 320;
		const float vision_min_range = 32;

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

void EnemyPatroller::draw() {
	//cout << "walker frame " << currentAnimation->getFrame() << "\n";
	AnimatedActor::draw();
}

void EnemyPatroller::die() {
	setCanCollide(false);
	dying = true;
	freeze();
	setCurrentAnimation("die");
	fireSound->playSound();
}

void EnemyPatroller::onAnimationComplete(std::string anim) {
	//cout << "EnemyPatroller::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		destroy();
		CollectibleEnergyBall * ball = new CollectibleEnergyBall(pos_x-16, pos_y-16);
	}

	if(anim == "hurt") {
		setCurrentAnimation("walk");
	}
}

void EnemyPatroller::onBumperCollide(int facing) {
	//cout << "Bumper collide " << actorName << "\n";
	if(facing == Facing::Left) {
		facing_direction = Facing::Right;
	} else if(facing == Facing::Right) {
		facing_direction = Facing::Left;
	}
} 
