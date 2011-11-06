/*
 * BlasterBullet.h
 *
 *  Created on: 04.11.2011
 *      Author: nehmulos
 */

#pragma once

#include "PlayerBullet.h"

class BlasterBullet : public PlayerBullet
{
public:
	BlasterBullet(double x, double y, int facing, double angleVariation, double lifetime = 1500);
	virtual ~BlasterBullet();

	void collideGround();
	void collideWall();
};

