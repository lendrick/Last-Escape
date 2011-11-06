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

#ifndef PLAYER_H
#define PLAYER_H

#include "Weapon.h"
#include "Map.h"
#include "AnimatedActor.h"
#include "Sound.h"
#include "globals.h"
#include "StartPoint.h"
#include "Blaster.h"
#include "ChargerWeapon.h"


class Player : public AnimatedActor {
public:
	Player(double x, double y);

	virtual bool isPlayer() { return true; }

	virtual void update(sf::Uint32 dt);
	virtual void draw();
	virtual void die();
	//virtual void collide(Actor & otherActor);
	virtual void onColliding(Actor & otherActor);
	virtual void onDestroy();
	virtual void onAnimationComplete(std::string anim);
	virtual bool doDamage(double damage, bool knockback = true);
	virtual void onDamage();
	virtual void onLevelUp(int newLevel);
	virtual void resetPhysicsCustom(double start_x, double start_y);
	
	void addExperience(int exp);
	int getCurrentExperience();
	
	bool hasKeycard(std::string name);
	void removeKeycard(std::string name);
	void addKeycard(std::string name);

	StartPoint * findStart();
	
	StartPoint * currentStart;
	void init();

	void jump(double dt);
	void crouch();

	void upgradeWeapon();

	//int currentWeapon;

	double time;
	double last_shoot_time;
	double last_jump_time;

	double energy;
	double energy_max;

	double speed_x;
	double speed_y;
	
	double shoot_duration;
	
	bool crouched;
	bool walking;
	
	int armor;
	int lives;
	int energyBalls;
	double recoveryTime;
	double recoveryTimer;
	
	double baseMaxEnergy;
	double energyPerLevel;
	
	Sound * fireSound;
	Sound * jumpSound;
	Sound * dieSound;
	
	int currentExperience;

private:
	bool wasShooting;

	list<std::string> keycards;

};


#endif
