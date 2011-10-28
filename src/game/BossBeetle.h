/*
 * BossBeetle.h
 *
 *  Created on: 27.10.2011
 *      Author: nehmulos
 */

#pragma once

#include "EnemyPatroller.h"
#include "SoundCache.h"

class BossBeetle : public EnemyPatroller
{
public:
	BossBeetle(double x, double y);
	virtual ~BossBeetle();

	void doUpdate(sf::Uint32);
	bool doDamage(double damage);
	void onDamage();
	void onBumperCollide(int facing);
	void collideGround();
private:
	sf::Uint32 time;
	sf::Uint32 dizzynessTime;
	int dizzynessTimer;
	Facing::Enum nextDirection;
};

