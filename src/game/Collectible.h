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

#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "AnimatedActor.h"
#include "Sound.h"
#include "SoundCache.h"

class Collectible : public AnimatedActor
{
public:
	Collectible(double x, double y, double w, double h);
	
	Sound* fireSound;

	virtual bool isCollectible() { return true; }
	virtual void reset();
protected:
	void init();
};

class CollectiblePill : public Collectible
{
public:
	CollectiblePill(double x, double y);
	virtual void collide(Actor& otherActor);
};

class CollectibleWeaponUpgrade : public Collectible
{
public:
	CollectibleWeaponUpgrade(double x, double y);
	virtual void collide(Actor& otherActor);
};


class CollectibleArmor : public Collectible
{
public:
	CollectibleArmor(double x, double y);
	virtual void collide(Actor& otherActor);
};

class CollectibleEnergyBall : public Collectible
{
public:
	CollectibleEnergyBall(double x, double y, int expValue = 1);
	virtual void collide(Actor& otherActor);
};

#endif
