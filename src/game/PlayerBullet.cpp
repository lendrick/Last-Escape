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

#include "PlayerBullet.h"
#include "Player.h"
#include "Enemy.h"
#include "globals.h"

PlayerBullet::PlayerBullet(double x, double y, int facing, double angleVariation, double lifetime):
AnimatedActor(x, y, 15.0f, 7.0f)
{
	this->setImage("xeon-bullet.png");
	this->lifetime = lifetime;
	bulletTime = 0;

	facing_direction = facing;

	double speed = 500.f;

	double angle = ((facing_direction == Facing::Right) ? 90 : -90) + (rand() % 200 - 100) * angleVariation/100;
	double speed_x = (int)(0.5f + sin(angle * 3.14159/180.0) * speed);
	double speed_y = (int)(0.5f + cos(angle * 3.14159/180.0) * speed);
	
	body->v.x = speed_x;
	body->v.y = speed_y;
	
	if(facing_direction == Facing::Right) 
		body->a = deg2rad(angle - 90);
	else
		body->a = deg2rad(angle - 270);
	
	setVelocityFunc(no_gravity);
	
	// Put it in the PLayerBullets group so it doesn't collide with other PlayerBullets
	shape->group = PhysicsGroup::PlayerBullets;
	shape->layers = PhysicsLayer::Map|PhysicsLayer::PlayerBullet;
	shape->collision_type = PhysicsType::PlayerBullet;
	
	setDrawOffset(8, 13);
	setFrameSize(16, 16);
	damage = 1;

	Animation * tmp;

	tmp = addAnimation("bullet");
	tmp->addFrame(0, .1f);
	tmp->addFrame(1, .1f);
	tmp->addFrame(2, .1f);
	tmp->addFrame(3, .1f);
	tmp->addFrame(4, .1f);
	tmp->setDoLoop(true);
	
	canSleep = false;

	setCurrentAnimation("bullet");
}

void PlayerBullet::collide(Actor& otherActor) {
	if (otherActor.isEnemy())
	{
		dynamic_cast<Enemy * >(&otherActor)->doDamage(damage);
		//if(dynamic_cast<Enemy * >(&otherActor)->doDamage(damage)) {
			//g_player->addExperience(otherActor.getExperienceValue());
		//}
		die();
	}
}

void PlayerBullet::update(double dt) {
	//cout << "bullet " << body->p.x << " " << body->p.y << "\n";
	bulletTime += dt;
	//cout << "angle " << rad2deg(body->a) << "\n";

	updateSpriteFacing();
	//checkcollisions();
	if(bulletTime > lifetime) {
		destroy();
	}
}

void PlayerBullet::collideGround() {
	destroy();
}
