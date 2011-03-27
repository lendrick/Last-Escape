#include "Enemy.h"

#include "Player.h"
#include "Sound.h"
#include "SoundCache.h"

Enemy::Enemy()
:AnimatedActor()
{
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
	} else if(!game_map->isSolid(pos_x + check_pos, pos_y + yOrigin + 1)) {
		// Turn around if there's a pit up ahead.
		//cout << "pit\n";
		//debugPixel.SetPosition(
		//	0.5f + (int)(pos_x - game_map->cam_x),
		//											 0.5f + (int)(pos_y - game_map->cam_y));
		//checkGround = true;
		flipDirection();
	}
}