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
#include "ChargerBullet.h"

ChargerBullet::ChargerBullet(double x, double y, int facing, double size, int damage, double angleVariation, double lifetime)
: PlayerBullet(x, y, size, size, facing, lifetime)
{
	this->setImage("energyball.png");
	double speed = 500.f;
	sprite.SetScale(1.0/(32.0/size), 1.0/(32.0/size));

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

	setDrawOffset(16, 16);
	setFrameSize(32, 32);
	this->damage = damage;

	Animation * tmp;

	tmp = addAnimation("bullet");
	tmp->addFrame(0, sf::Milliseconds(100));
	tmp->addFrame(1, sf::Milliseconds(100));
	tmp->addFrame(2, sf::Milliseconds(100));
	tmp->addFrame(3, sf::Milliseconds(100));
	tmp->addFrame(4, sf::Milliseconds(100));
	tmp->addFrame(5, sf::Milliseconds(100));
	tmp->addFrame(6, sf::Milliseconds(100));
	tmp->addFrame(7, sf::Milliseconds(100));
	tmp->addFrame(8, sf::Milliseconds(100));
	tmp->setDoLoop(true);

	setCurrentAnimation("bullet");
	this->setColor(sf::Color(230,200,0,255));
}

ChargerBullet::~ChargerBullet()
{
	// TODO Auto-generated destructor stub
}
