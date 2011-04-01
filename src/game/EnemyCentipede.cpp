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

#include "EnemyCentipede.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"
#include "Collectible.h"
#include "ImageCache.h"
#include <SFML/Graphics.hpp>

EnemyCentipede::EnemyCentipede(float x, float y)
:EnemyPatroller(x, y, 32.0f, 25.0f)
{
	//debugPixel.SetImage(*imageCache["bluepixel.png"]);
	setImage("centipede.png");
	walk_speed = 90.f;
	shootInterval = 2.5f;
	lastShot = 0;
	time = 0;

	dying = false;

	setDrawOffset(32, 30);
	setFrameSize(64, 32);

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

	Animation * tmp;

	tmp = addAnimation("walk");
	tmp->addFrame(2, .2f);
	tmp->addFrame(3, .2f);
	tmp->addFrame(6, .2f);
	tmp->addFrame(7, .2f);
	tmp->setDoLoop(true);

	tmp = addAnimation("die");
	tmp->addFrame(0, .07f);
	tmp->addFrame(4, .07f);
	tmp->addFrame(8, .07f);
	tmp->addFrame(12, .07f);

	tmp = addAnimation("shoot");
	tmp->addFrame(11, .4f);
	tmp->addFrame(10, .2f);
	tmp->addFrame(9, .1f);

	setCurrentAnimation("walk");
}

void EnemyCentipede::update(float dt) {
	if(!dying) {
		time += dt;
		if(lastShot + shootInterval <= time && animationName() != "shoot") {
			setCurrentAnimation("shoot");
			shape->u = 1.0f;
		} else {
			shape->u = 0.1f;
		}
		
		if(animationName() != "shoot")
			EnemyPatroller::update(dt);
	}
}

void EnemyCentipede::onAnimationComplete(std::string anim) {
	//cout << "EnemyCentipede::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		destroy();
		CollectibleEnergyBall * ball = new CollectibleEnergyBall(pos_x, pos_y-16);
	} else if(anim == "shoot") {
		//shoot a projectile
		lastShot = time;
		setCurrentAnimation("walk");
		int px = pos_x + 8;
		if(facing_direction == Facing::Left) px -= 16;
		EnemyCentipedeProjectile * projectile =
			new EnemyCentipedeProjectile(facing_direction, px, int(pos_y - 20.0f));
	}
}

EnemyCentipedeProjectile::EnemyCentipedeProjectile(int direction, int start_x, int start_y)
	: Enemy(start_x, start_y, 8.0f, 4.0f)
{
	setImage("centipedeprojectile.png");
	fly_speed = 400.0f;
	speed_y = 200.0f;
	dying = false;

	setDrawOffset(16, 11);
	setFrameSize(32, 16);
	facing_direction = direction;
	if(facing_direction == Facing::Left) {
		speed_x = -fly_speed;
	} else {
		speed_x = fly_speed;
	}

	Animation * tmp;

	tmp = addAnimation("fly");
	tmp->addFrame(8, .1f);
	tmp->addFrame(9, .1f);
	tmp->addFrame(8, .1f);
	tmp->addFrame(10, .1f);
	tmp->setDoLoop(true);

	tmp = addAnimation("splat");
	tmp->addFrame(12, .2f);
	tmp->addFrame(13, .5f);

	setCurrentAnimation("fly");
	
	body->v = cpv(speed_x, speed_y);

	shape->layers = PhysicsLayer::EnemyBullet;
	shape->group = PhysicsGroup::EnemyBullets;
}

void EnemyCentipedeProjectile::update(float dt)
{
	if(isGrounded()) {
		setCurrentAnimation("splat");
	}
}

void EnemyCentipedeProjectile::collide(Actor &otherActor) {
	destroy();
}

void EnemyCentipedeProjectile::onAnimationComplete(std::string anim)
{
	if(anim == "splat") {
		destroy();
	}
}
