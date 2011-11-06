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

#include "ChargerWeapon.h"
#include "Player.h"

ChargerWeapon::ChargerWeapon()
{
	upgrade = 0;
	maxUpgrade = 2;

	chargePercentage = 0;
	minChargePercentage = 0.1f;
	startChargeTime = 0;
	maxChargeTime = 1500;

	chargingShot.SetScale(0,0);
	chargingShot.SetOrigin(16,16);
	chargingShot.SetTexture(*imageCache["energyball.png"]);
	chargingShot.SetSubRect(sf::IntRect(0,0,32,32));
	chargingShot.SetColor(sf::Color(230,200,0,255));
	charginShotRotation = 0;
	chargingSound = soundCache["charging.oga"];


	std::string tempName[] = {"Charger", "Big Charger", "Hyper Charger"};
	name.assign( tempName, tempName + 3 );

	double tempEnergyCost[] = {30.f, 20.f, 10.f};
	energy_cost.assign( tempEnergyCost, tempEnergyCost + 3 );

	double tempReloadTime[] = {500, 100, 10};
	reload_time.assign( tempReloadTime, tempReloadTime + 3 );

	double tempMaxDamage[] = {2, 2, 3};
	maxDamage.assign(tempMaxDamage, tempMaxDamage + 3);

	sprite_row.assign( 3, 2 );

	sprite_count.assign( 3, 3 );

	double tempSpriteSpeed[] = {16000, 32000, 32000};
	sprite_speed.assign( tempSpriteSpeed, tempSpriteSpeed + 3 );

	//std::cout << "Upgrade: " << upgrade << "\nName: " << name[upgrade] << "\nEnergy Cost: " << energy_cost[upgrade] << "\nReload Time: " << reload_time[upgrade]
		//<< "\nAngle Variation: " << angle_variation[upgrade] << "\nSprite Speed: " << sprite_speed[upgrade];

}

ChargerWeapon::~ChargerWeapon()
{
}

void ChargerWeapon::reset()
{
	upgrade = 0;
}
void ChargerWeapon::upgradeWeapon()
{

}
void ChargerWeapon::startShooting(Player& player)
{
	startChargeTime = player.time;
//	player.last_shoot_time = player.time;
}
void ChargerWeapon::shooting(Player & player)
{
	sf::Uint32 chargeTime = player.time - startChargeTime;
	chargeTime = min(chargeTime,maxChargeTime);
	float chargeAdded = ((float)chargeTime/(float)maxChargeTime) - chargePercentage;
	chargePercentage += chargeAdded;

	if ( player.energy < energy_cost[upgrade]*chargeAdded)
	{
		stopShooting(player);
		return;
	}
	player.energy -= energy_cost[upgrade]*chargeAdded;

	chargingSound->setVolume(50.f*chargePercentage);
	if(chargingSound->getStatus() == sf::Sound::Stopped)
		chargingSound->playSound();
}
void ChargerWeapon::stopShooting(Player& player)
{
	if(chargePercentage < minChargePercentage)
	{
		chargePercentage = 0.f;
		return;
	}

	player.last_shoot_time = player.time;
	player.fireSound->playSound();

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
		bulletY = player.body->p.y - 6.0f;
	}
	else
	{
		bulletY = player.body->p.y + 6.0f;
	}

	int damage = max((int)(maxDamage[upgrade]*chargePercentage), 1);
	Actor * bullet = new ChargerBullet( bulletX, bulletY, player.getFacing(), 32.f*chargePercentage, damage, 0, 1500*chargePercentage);
	chargePercentage = 0.f;
	chargingShot.SetScale(0,0);
	chargingSound->stop();

	if ( player.walking )
	{
		player.setCurrentAnimation("walkshoot");
	}
	else
	{
		player.setCurrentAnimation("shoot");
	}
}

void ChargerWeapon::draw(Player& player)
{
	// update the chargingShot graphics
	if(chargePercentage >= minChargePercentage)
	{
		double bulletX, bulletY;

		if ( player.getFacing() == Facing::Left )
		{
			bulletX = player.body->p.x - 20.0f;
		}
		else if ( player.getFacing() == Facing::Right )
		{
			bulletX = player.body->p.x + 20.0f;
		}

		if ( player.crouched )
		{
			bulletY = player.body->p.y - 6.0f;
		}
		else
		{
			bulletY = player.body->p.y + 3.0f;
		}
		chargingShot.SetPosition(bulletX, bulletY);
		double size = 32.f*chargePercentage;
		chargingShot.SetScale(size/32.0, size/32.0);
		chargingShot.Rotate(-3);
		sf::Vector2f newPos = rotateVector(chargingShot.GetSize().x/2, chargingShot.GetSize().y/2, charginShotRotation, chargingShot.GetPosition());
		chargingShot.SetPosition(newPos);

		charginShotRotation += 0.1;
		if(charginShotRotation > 360)
			charginShotRotation = 0;

		App->Draw(chargingShot);
	}
}
