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

#ifndef WEAPON_H
#define WEAPON_H

#include "PlayerBullet.h"
#include <string>
#include <list>
#include <vector>

class Player;

class Weapon
{
	public:

	virtual void reset() = 0;
	virtual void upgradeWeapon() = 0;
	virtual void startShooting(Player& player) = 0;
	virtual void shooting(Player & player) = 0;
	virtual void stopShooting(Player& player) = 0;
	virtual ~Weapon() {}

	int getUpgrade() const { return upgrade; }
	std::string getName() const { return name[upgrade]; }
	double getEnergyCost() const { return energy_cost[upgrade]; }
	double getReloadTime() const { return reload_time[upgrade]; }

	protected:

	int upgrade;
	int maxUpgrade;
	std::vector<std::string> name;
	std::vector<double> energy_cost;
	std::vector<double> reload_time;
};

#endif
