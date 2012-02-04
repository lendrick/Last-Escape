/*
 * BlasterBullet.cpp
 *
 *  Created on: 04.11.2011
 *      Author: nehmulos
 */

#include "BlasterBullet.h"

BlasterBullet::BlasterBullet(double x, double y, int facing, double angleVariation, double lifetime)
: PlayerBullet(x, y, 15.0f, 7.0f, facing, lifetime)
{
	this->setImage("xeon-bullet.png");
	double speed = 500.f;

	double angle = ((facing_direction == Facing::Right) ? 90 : -90) + (rand() % 200 - 100) * angleVariation/100;
	double speed_x = (int)(0.5f + sin(angle * 3.14159/180.0) * speed);
	double speed_y = (int)(0.5f + cos(angle * 3.14159/180.0) * speed);

	body->v.x = speed_x;
	body->v.y = speed_y;

	if(facing_direction == Facing::Right)
		body->a = deg2rad(-angle - 270);
	else
		body->a = deg2rad(-angle - 90);

	setVelocityFunc(no_gravity);

	setDrawOffset(8, 6);
	setFrameSize(16, 16);
	damage = 1;

	Animation * tmp;

	tmp = addAnimation("bullet");
	tmp->addFrame(0, sf::Milliseconds(100));
	tmp->addFrame(1, sf::Milliseconds(100));
	tmp->addFrame(2, sf::Milliseconds(100));
	tmp->addFrame(3, sf::Milliseconds(100));
	tmp->addFrame(4, sf::Milliseconds(100));
	tmp->setDoLoop(true);

	setCurrentAnimation("bullet");
}

BlasterBullet::~BlasterBullet()
{
	// TODO Auto-generated destructor stub
}


void BlasterBullet::collideGround() {
	destroy();
}

void BlasterBullet::collideWall() {
	destroy();
}
