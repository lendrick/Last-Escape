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

#include "Enemy.h"

#include "Player.h"
#include "Sound.h"
#include "SoundCache.h"

Enemy::Enemy(double x, double y, double w, double h)
  :AnimatedActor(x, y, w, h)
{
  //make sure we die in one hit by default
  life = 0.1f;
  experienceValue = 1;
  actorName = "Enemy";
	shapeLayers = PhysicsLayer::Map|PhysicsLayer::Enemy|PhysicsLayer::PlayerBullet;
	collisionType = PhysicsType::Enemy;
}


void Enemy::collideCallback(Actor& otherActor)
{
  (void)otherActor;
  /*
  if (otherActor.isCollectible())
  	otherActor.destroy();
  */

  /*
  if (otherActor.isPlayer())
  {
  	// TODO: kill the player
  	otherActor.sprite.FlipY(true);
  }
  */
}

