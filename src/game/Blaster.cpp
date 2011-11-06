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
#include "Player.h"

Blaster::Blaster()
{
	upgrade = 0;
	maxUpgrade = 2;

	std::string tempName[] = {"Blaster", "Overcharged Blaster", "Way Overcharged Blaster"};
	name.assign( tempName, tempName + 3 );

	double tempEnergyCost[] = {5.0f, 2.5f, 2.0f};
	energy_cost.assign( tempEnergyCost, tempEnergyCost + 3 );

	double tempReloadTime[] = {500, 100, 10};
	reload_time.assign( tempReloadTime, tempReloadTime + 3 );

	double tempAngleVariation[] = {0.0f, 10.0f, 5.0f};
	angle_variation.assign( tempAngleVariation, tempAngleVariation + 3 );

	sprite_row.assign( 3, 2 );

	sprite_count.assign( 3, 3 );

	double tempSpriteSpeed[] = {16000, 32000, 32000};
	sprite_speed.assign( tempSpriteSpeed, tempSpriteSpeed + 3 );
	
	//std::cout << "Upgrade: " << upgrade << "\nName: " << name[upgrade] << "\nEnergy Cost: " << energy_cost[upgrade] << "\nReload Time: " << reload_time[upgrade]
		//<< "\nAngle Variation: " << angle_variation[upgrade] << "\nSprite Speed: " << sprite_speed[upgrade];
}

Blaster::~Blaster() {}

void Blaster::upgradeWeapon()
{
	if ( upgrade < maxUpgrade )
	{
		++upgrade;
		
		//std::cout << "Upgrade: " << upgrade << "\nName: " << name[upgrade] << "\nEnergy Cost: " << energy_cost[upgrade] << "\nReload Time: " << reload_time[upgrade]
		//<< "\nAngle Variation: " << angle_variation[upgrade] << "\nSprite Speed: " << sprite_speed[upgrade];
	}
}

void Blaster::reset()
{
	upgrade = 0;
	
	//std::cout << "Upgrade: " << upgrade << "\nName: " << name[upgrade] << "\nEnergy Cost: " << energy_cost[upgrade] << "\nReload Time: " << reload_time[upgrade]
		//<< "\nAngle Variation: " << angle_variation[upgrade] << "\nSprite Speed: " << sprite_speed[upgrade];
}

void Blaster::shooting(Player & player)
{
	if ( player.energy < energy_cost[upgrade] )
	{
		return;
	}

	if ( (player.time - player.last_shoot_time) > reload_time[upgrade] )
	{
		player.last_shoot_time = player.time;
		player.fireSound->playSound();
		player.energy -= energy_cost[upgrade];

		double bulletX, bulletY;

		if ( player.getFacing() == Facing::Left )
		{
			bulletX = player.body->p.x - 30.0f;
		}
		else if ( player.getFacing() == Facing::Right )
		{
			bulletX = player.body->p.x + 30.0f;
		}

		if ( player.crouched )
		{
			bulletY = player.body->p.y - 9.0f;
		}
		else
		{
			bulletY = player.body->p.y + 6.0f;
		}

		Actor * bullet = new BlasterBullet( bulletX, bulletY, player.getFacing(), angle_variation[upgrade] );

		if ( player.walking )
		{
			player.setCurrentAnimation("walkshoot");
		}
		else
		{
			player.setCurrentAnimation("shoot");
		}
	}
}		
