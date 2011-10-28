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

#include "Collectible.h"

#include "Player.h"
#include "globals.h"
#include "Sound.h"
#include "SoundCache.h"

Collectible::Collectible(double x, double y, double w, double h)
:AnimatedActor(x, y, w, h, true)
{
	shape->sensor = true;
}


void Collectible::init()
{
	setDrawOffset(16, 16);
	setFrameSize(32, 32);

	Animation * tmp;
	tmp = addAnimation("image");
	tmp->addFrame(0, 100);
	setCurrentAnimation("image");
	shape->collision_type = PhysicsType::Item;
	setShapeLayers(PhysicsLayer::Player);
	actorName = "Collectible";
}

void Collectible::reset() {
	setCanCollide(true);
	hidden = false;
}

CollectiblePill::CollectiblePill(double x, double y)
:Collectible(x, y, 32.0f, 32.0f)
{
	this->setImage("smallenergy.png");
	actorName = "Pill";
	init();
}

void CollectiblePill::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		fireSound = soundCache["energyblip.ogg"];
		fireSound->playSound();
		setCanCollide(false);
		hidden = true;
        // give player 5 energy, but only up to max energy + 100
        if(((Player&)otherActor).energy < ((Player&)otherActor).energy_max + 100) {
            ((Player&)otherActor).energy = min(((Player&)otherActor).energy_max + 100, ((Player&)otherActor).energy + 5.0f);
        }
	}
}


CollectibleWeaponUpgrade::CollectibleWeaponUpgrade(double x, double y)
:Collectible(x, y, 32.0f, 32.0f)
{
	actorName = "Weapon Upgrade";
	this->setImage("plasmaball.png");
	init();
}

void CollectibleWeaponUpgrade::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		soundCache["gmae.ogg"]->playSound();
		setCanCollide(false);
		hidden = true;
		((Player&)otherActor).upgradeWeapon();
	}
}


CollectibleArmor::CollectibleArmor(double x, double y)
:Collectible(x, y, 32.0f, 32.0f) {
	actorName = "Armor";
	this->setImage("shield.png");
	init();
}

void CollectibleArmor::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		setCanCollide(false);
		hidden = true;
		((Player&)otherActor).armor += 1;
	}
}


CollectibleEnergyBall::CollectibleEnergyBall(double x, double y, int expValue)
: Collectible(x, y, 32.0f, 32.0f) {
	actorName = "EnergyBall";
	this->setImage("energyball.png");
	setDrawOffset(16, 16);
	setFrameSize(32, 32);
	experienceValue = expValue;

	Animation * tmp;
	tmp = addAnimation("anim");
	tmp->addFrame(0, 200);
	tmp->addFrame(1, 200);
	tmp->addFrame(2, 200);
	tmp->addFrame(3, 200);
	tmp->setDoLoop(true);
	setCurrentAnimation("anim");
	shape->collision_type = PhysicsType::Item;
	setShapeLayers(PhysicsLayer::Player);
}

void CollectibleEnergyBall::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		if(!g_player->dying) {
			destroy();
			soundCache["gmae.ogg"]->playSound();
			if(g_player->energyBalls < 10)
                g_player->energyBalls++;

			g_player->addExperience(experienceValue);
			// give player 25 energy, but only up to max energy
			if(((Player&)otherActor).energy < ((Player&)otherActor).energy_max) {
				((Player&)otherActor).energy = min(((Player&)otherActor).energy_max, ((Player&)otherActor).energy + 25.f);
			}
		}
	}
}

CollectibleKeycard::CollectibleKeycard(double x, double y, std::string name)
: Collectible(x, y, 32.0f, 41.0f) {
	actorName = "Keycard";
	this->name = name;
	if(!setImage("keycard-" + name + ".png"))
		setImage("keycard.png");

	setDrawOffset(16, 20);
	setFrameSize(32, 41);

	Animation * tmp;
	tmp = addAnimation("be a keycard");
	tmp->addFrame(0, 9001);
	setCurrentAnimation("be a keycard");
	shape->collision_type = PhysicsType::Item;
	setShapeLayers(PhysicsLayer::Player);
}

void CollectibleKeycard::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		Player& player = (Player&)otherActor;
		if(!player.dying)
		{
			destroy();
			soundCache["gmae.ogg"]->playSound();
			player.addKeycard(name);
		}
	}
}
