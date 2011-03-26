#include "Enemy.h"

#include "Player.h"

Enemy::Enemy()
:AnimatedActor()
{
}

Enemy::~Enemy()
{
}

void Enemy::collide(Actor& otherActor) {
	if (otherActor.isCollectible())
		otherActor.destroy();
	
	if (otherActor.isPlayer())
	{
		// TODO: kill the player
		otherActor.sprite.FlipY(true);
	}
}

