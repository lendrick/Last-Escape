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

#include "Blaster.h"

Blaster::Blaster()
{
	upgrade = 1;
	maxUpgrade = 3;

	std::string tempName[] = {"Blaster", "Overcharged Blaster", "Way Overcharged Blaster"};
	name.assign( tempName, tempName + 3 );

	double tempEnergyCost[] = {5.0f, 2.5f, 2.0f};
	energy_cost.assign( tempEnergyCost, tempEnergyCost + 3 );

	double tempReloadTime[] = {0.5f, 0.1f, 0.01f};
	reload_time.assign( tempReloadTime, tempReloadTime + 3 );

	double tempAngleVariation[] = {0.0f, 10.0f, 5.0f};
	angle_variation.assign( tempAngleVariation, tempAngleVariation + 3 );

	sprite_row.assign( 3, 2 );

	sprite_count.assign( 3, 3 );

	double tempSpriteSpeed[] = {16.0f, 32.0f, 32.0f};
	sprite_speed.assign( tempSpriteSpeed, tempSpriteSpeed + 3);
}

Blaster::~Blaster() {}

void Blaster::upgrade()
{
	if ( upgrade > maxUpgrade )
	{
		++upgrade;
	}
}

