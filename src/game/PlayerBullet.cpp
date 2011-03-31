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

PlayerBullet::PlayerBullet(float x, float y, int facing, float angleVariation, float lifetime):
AnimatedActor(x, y, 15.0f, 7.0f)
{
	this->setImage("xeon-bullet.png");
	this->lifetime = lifetime;
	bulletTime = 0;

	facing_direction = facing;

	float speed = 480.f;

	float angle = ((facing_direction == Facing::Right) ? 90 : -90) + (rand() % 200 - 100) * angleVariation/100;
	speed_x = (int)(0.5f + sin(angle * 3.14159/180.0) * speed);
	speed_y = (int)(0.5f + cos(angle * 3.14159/180.0) * speed);

	setDrawOffset(8, 9);
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

	setCurrentAnimation("bullet");
	shape->layers = PhysicsLayer::Map|PhysicsLayer::Enemy;
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

void PlayerBullet::update(float dt) {
	bulletTime += dt;
	float mx = speed_x*dt;
	float my = speed_y*dt;
	bool impact = move(mx, my);
	updateSpriteFacing();
	//checkcollisions();
	if(impact || bulletTime > lifetime) {
		destroy();
	}
}
