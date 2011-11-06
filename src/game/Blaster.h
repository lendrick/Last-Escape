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

#ifndef BLASTER_H
#define BLASTER_H

#include "Weapon.h"
#include "BlasterBullet.h"

class Blaster : public Weapon
{
	public:

	Blaster();
	~Blaster();

	void reset();
	void upgradeWeapon();
	void shooting(Player & player);
	void startShooting(Player & player) {};
	void stopShooting(Player & player) {};
	void draw(Player& player) {};

	protected:

	std::vector<double> angle_variation;
	std::vector<int> sprite_row;
	std::vector<int> sprite_count;
	std::vector<sf::Uint32> sprite_speed;

};

#endif
