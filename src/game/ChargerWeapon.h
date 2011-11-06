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
#pragma once

#include "Weapon.h"
#include "ChargerBullet.h"
#include "ImageCache.h"
#include "SoundCache.h"

class ChargerWeapon : public Weapon
{
public:
	ChargerWeapon();
	virtual ~ChargerWeapon();

	virtual void reset();
	virtual void upgradeWeapon();
	virtual void startShooting(Player& player);
	virtual void shooting(Player & player);
	virtual void stopShooting(Player& player);
	virtual void draw(Player& player);

protected:
	float charginShotRotation;
	float chargePercentage;
	float minChargePercentage;
	sf::Uint32 startChargeTime;
	sf::Uint32 maxChargeTime;
	sf::Sprite chargingShot;

	std::vector<int> sprite_row;
	std::vector<int> sprite_count;
	std::vector<int> maxDamage;
	std::vector<sf::Uint32> sprite_speed;
	Sound* chargingSound;

};

