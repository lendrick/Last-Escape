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

PlayerBullet::PlayerBullet(double x, double y, double w, double h, int facing, double lifetime):
AnimatedActor(x, y, w, h)
{
	this->lifetime = lifetime;
	bulletTime = 0;

	facing_direction = facing;

	// Put it in the PLayerBullets group so it doesn't collide with other PlayerBullets
	shape->group = PhysicsGroup::PlayerBullets;
	shape->layers = PhysicsLayer::Map|PhysicsLayer::PlayerBullet;
	shape->collision_type = PhysicsType::PlayerBullet;

	canSleep = false;
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
	else if ( dynamic_cast<PhysicsCrate*>(&otherActor) != NULL )
	{
		((PhysicsCrate&)otherActor).doDamage(damage);
		die();
	}
}

void PlayerBullet::update(sf::Uint32 dt) {
	//cout << "bullet " << body->p.x << " " << body->p.y << "\n";
	bulletTime += dt;
	//cout << "angle " << rad2deg(body->a) << "\n";

	updateSpriteFacing();
	//checkcollisions();
	if(bulletTime > lifetime) {
		destroy();
	}
}

