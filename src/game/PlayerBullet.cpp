#include "PlayerBullet.h"

#include "Enemy.h"

PlayerBullet::PlayerBullet(bool rightwards):
AnimatedActor()
{
	bullet_speed = (rightwards ? +1 : -1) * 480.f;
	setPlaceholder(sf::Color(0, 255, 0), 8, 8);
}

void PlayerBullet::collide(Actor& otherActor) {
	if (otherActor.isEnemy())
		otherActor.destroy();
}

void PlayerBullet::update(float dt) {
	move(bullet_speed*dt, 0);
}
