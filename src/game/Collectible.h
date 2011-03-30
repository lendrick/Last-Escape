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
	Collectible();
	
	Sound* fireSound;
	SoundCache soundCache;

	virtual bool isCollectible() { return true; }
	virtual void reset();
protected:
	void init();
};

class CollectiblePill : public Collectible
{
public:
	CollectiblePill();
	virtual void collide(Actor& otherActor);
};

class CollectibleWeaponUpgrade : public Collectible
{
public:
	CollectibleWeaponUpgrade();
	virtual void collide(Actor& otherActor);
};


class CollectibleArmor : public Collectible
{
public:
	CollectibleArmor();
	virtual void collide(Actor& otherActor);
};

class CollectibleEnergyBall : public Collectible
{
public:
	CollectibleEnergyBall(int expValue = 1);
	virtual void collide(Actor& otherActor);
};

#endif
