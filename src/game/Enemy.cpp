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

Enemy::Enemy(float x, float y)
:AnimatedActor(x, y)
{
	//make sure we die in one hit by default
	life = 0.1f;
}


void Enemy::collide(Actor& otherActor) {

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


void Enemy::patrol(float dt) {
	float delta_x = speed_x * dt;
	float delta_y = speed_y * dt;

	int check_pos = 12;
	if(facing_direction == FACING_LEFT) check_pos = -check_pos;
	if(move(delta_x, delta_y)) {
		// Turn around if you run into something.
		//cout << "obstructed\n";
		flipDirection();
	} else if(!game_map->isSolid(int(pos_x + check_pos), int(pos_y + yOrigin + 1))) {
		// Turn around if there's a pit up ahead.
		//cout << "pit\n";
		//debugPixel.SetPosition(
		//	0.5f + (int)(pos_x - game_map->cam_x),
		//											 0.5f + (int)(pos_y - game_map->cam_y));
		//checkGround = true;
		flipDirection();
	}
}
