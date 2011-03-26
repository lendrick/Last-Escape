#include "PlayerBullet.h"

#include "Enemy.h"

PlayerBullet::PlayerBullet(int facing, float angleVariation):
AnimatedActor()
{
	facing_direction = facing;

	float speed = 480.f;

	float angle = ((facing_direction == FACING_RIGHT) ? 90 : -90) + (rand() % 100) * angleVariation/100;
	speed_x = sin(angle * 3.14159/180.0) * speed;
	speed_y = cos(angle * 3.14159/180.0) * speed;
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
	move(speed_x*dt, speed_y*dt);

	checkCollisions();
}
