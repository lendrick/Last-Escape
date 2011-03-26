#include "PlayerBullet.h"

#include "Enemy.h"

PlayerBullet::PlayerBullet(int facing):
AnimatedActor()
{
	facing_direction = facing;
	bullet_speed = ((facing_direction == FACING_RIGHT) ? +1 : -1) * 480.f;
	setPlaceholder(sf::Color(0, 255, 0), 8, 8);
}

void PlayerBullet::collide(Actor& otherActor) {
	if (otherActor.isEnemy())
	{
		otherActor.destroy();
		destroy();
	}
}

void PlayerBullet::update(float dt) {
	move(bullet_speed*dt, 0);

	checkCollisions();
}
