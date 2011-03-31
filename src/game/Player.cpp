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

#include "Player.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sound.h"
#include "globals.h"
#include "ImageCache.h"
#include "SoundCache.h"
#include "Ui.h"
#include "ExitPoint.h"
#include "Collectible.h"

const float energy_cost_jump = 0.f;
const float energy_recharge_rate = 5.f; // units per second
static const int start_lives = 3;

struct WeaponDesc {
	const char* name;
	float energy_cost;
	float reload_time;
	float angle_variation;

	int sprite_row;
	int sprite_count;
	float sprite_speed;
};

const int num_weapon_types = 3;
WeaponDesc weapons[num_weapon_types] = {
	{"Blaster", 5.0, 0.5, 0.0,
		2, 3, 16.0 },
	{"Overcharged Blaster", 2.5f, 0.1f, 10.0f,
		2, 3, 32.0f },
	{"Way Overcharged Blaster", 2.0f, 0.01f, 5.0f,
		2, 3, 32.0f },
};

Player::Player(float x, float y)
: AnimatedActor(x, y, 24, 48) {
	setImage("xeon.png");

	lives = start_lives;
	setDrawOffset(64, 104);
	setFrameSize(128, 128);
	shoot_duration = .2f;
	last_shoot_time = 0;
	energyBalls = 0;
	immunityTime = 0.5f;
	recoveryTime = 0.2f;
	recoveryTimer = 0;
	currentWeapon = 0;
	baseMaxEnergy = 100.0f;
	energyPerLevel = 10.0f;
	energy_max = baseMaxEnergy;
	currentExperience = 0;
	actorName = "Player";

	loadAnimationsFromFile("xeon.xml");
	armor = 0;

	fireSound = soundCache["shoot.ogg"];
	jumpSound = soundCache["ambient_techno1.ogg"];
	dieSound  = soundCache["xeonDies.ogg"];
	currentStart = NULL;
	dying = false;

	init();
}

void Player::init() {
	time = 0.f;
	last_shoot_time = -100.f;
	dying = false;

	energy = energy_max;

	if(currentStart == NULL)
		currentStart = findStart();

	float sx, sy;
	if(currentStart != NULL) {
		currentStart->getPos(sx, sy);
		setPos(sx, sy);
	}

	std::cout << "Init player at " << sx << ", " << sy << std::endl;

	facing_direction = Facing::Right;
	crouched = false;

	this->setCurrentAnimation("idle");
	resetPhysics();
	
	if(shape) {
		shape->layers = PhysicsLayer::Map|PhysicsLayer::Player|PhysicsLayer::Enemy;
		shape->collision_type = PhysicsType::Player;
	}
}

StartPoint * Player::findStart() {
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		if((*it)->isStartPoint() && !(*it)->isDestroyed()) {
			if (debugMode)
				cout << "found start at " << (*it)->pos_x << " " << (*it)->pos_y << "\n";
			return static_cast<StartPoint *>(*it);
		}
	}

	return NULL;
}

void Player::upgradeWeapon() {
	currentWeapon = min(currentWeapon+1, num_weapon_types-1);
}

void Player::jump(float dt) {
	const int jump_speed = 525;
	const float max_jet_accel = 2000;
	const float jet_cost = 60;
	const float jet_speed_max = 450;
	const float jet_wait = 0.4f;

	if (body->v.y == 0 && isGrounded())
	{
		if (energy < energy_cost_jump)
			return;

	  cpBodyApplyImpulse(body, cpv(0, 6000), cpv(0, 0));
		last_jump_time = time;
		jumpSound->playSound();
	}
	else
	{
		// Can't jet immediately after jumping
		if (time - last_jump_time < jet_wait)
			return;

		float cost = jet_cost * dt;
		if (energy < cost)
			return;

		energy -= cost;
		
		if(body->v.y < jet_speed_max)
			cpBodyApplyImpulse(body, cpv(0, 400), cpv(0, 0));
			
		if(jumpSound->getStatus() != sf::Sound::Playing)
			jumpSound->playSound();
	}
}

void Player::shoot() {
	const float shoot_reload_timer = 0.5f;

	if (energy < weapons[currentWeapon].energy_cost)
		return;

	if (time - last_shoot_time > weapons[currentWeapon].reload_time) {
		last_shoot_time = time;
		fireSound->playSound();
		energy -= weapons[currentWeapon].energy_cost;

		float bulletX = pos_x + 30.0f, bulletY = 0.0f;
		if(facing_direction == Facing::Left)
			bulletX = pos_x - 30.0f;

		if(crouched) {
			bulletY = pos_y - 15.0f;
		}
		else {
			bulletY = pos_y - 30.0f;
		}

		Actor* bullet = new PlayerBullet(bulletX, bulletY, facing_direction, weapons[currentWeapon].angle_variation);

		if(walking) {
			setCurrentAnimation("walkshoot");
		}
		else {
			setCurrentAnimation("shoot");
		}


		//resetCurrentAnimation();
	}
}

void Player::crouch() {
	crouched = true;
}

void Player::update(float dt) {
	if(!body) return;
	const int speed_max = 240; // pixels per second
	const int speed_delta = speed_max*4; // pixels per second per second
	const int speed_delta_decel = speed_max*4;
	const int terminal_velocity = 16 * 60;
	walking = false;
	if(recoveryTimer > 0) {
		recoveryTimer -= dt;
	} else {
		recoveryTimer = 0;
	}

	if(energyBalls == 10) {
		energyBalls = 0;
		lives++;
		soundCache["1up.ogg"]->playSound();
	}

	time += dt;

	if(game_map->isOnInstantdeath(*this)) {
		this->die();
	}

	int move_direction = Facing::None;

	// recharge energy
	if(energy > 0)
        energy += std::min(energy_recharge_rate*dt, std::max(0.f, energy_max - energy));

	if (godMode)
		energy = std::max(energy, 10.f);

	// left/right move
	shape->u = 2.0f;
	if (!dying && recoveryTimer <= 0 && input.direction() == Facing::Left) {
		move_direction = Facing::Left;
		if(body->v.x > -speed_max)
			cpBodyApplyImpulse(body, cpv(-500, 0), cpv(0, 0));
		facing_direction = move_direction;
		walking = true;
		shape->u = 0.1f;
	}
	else if (!dying && recoveryTimer <= 0 && input.direction() == Facing::Right) {
		move_direction = Facing::Right;
		if(body->v.x < speed_max)
			cpBodyApplyImpulse(body, cpv(500, 0), cpv(0, 0));
		facing_direction = move_direction;
		walking = true;
		shape->u = 0.1f;
	}

	if (!dying && recoveryTimer <= 0 && input.jumping())
		jump(dt);
	else if(!dying && recoveryTimer <= 0 && !input.jumping() && body->v.y > 0) {
		cpBodyApplyImpulse(body, cpv(0, -220), cpv(0, 0));
	}

	if (!dying && recoveryTimer <= 0 && input.shooting())
		shoot();

	if(!dying && recoveryTimer <= 0 && isGrounded() && input.crouching() && input.direction() != Facing::Left && input.direction() != Facing::Right)
    {
        //height = 32;
		crouched = true;
    }
	else
    {
        //height = 48;
		crouched = false;
    }

	// Compute animations:

	if(!dying) {
		if(recoveryTimer > 0) {
			this->setCurrentAnimation("damage");
		}
		if(time - last_shoot_time < shoot_duration) {
			if(walking) {
				this->setCurrentAnimation("walkshoot");
			} else if(crouched) {
				this->setCurrentAnimation("crouchshoot");
			} else {
				this->setCurrentAnimation("shoot");
			}
		}
		else if (!isGrounded() && body->v.y != 0)
		{
			if(body->v.y > 0)
				this->setCurrentAnimation("jump");
			else
				this->setCurrentAnimation("fall");
		}
		else if (walking)
		{
			if (debugMode)
				std::cout<< this->getCurrentAnimation()->getIsFinished();
			if(this->getCurrentAnimation()->getIsFinished()) {
				this->setCurrentAnimation("walk");
			}
		}
		else if(crouched) {
			this->setCurrentAnimation("crouch");
		}
		else
		{
			this->setCurrentAnimation("idle");
		}

		/*
		cout << (crouched?"crouched":"not-crouched") << " " 
		     << (isGrounded()?"grounded":"not-grounded") << " "
		     << currentAnimation->getName() << "\n";
	  */
		updateSpriteFacing();

		/*
		if (speed_y == terminal_velocity)
			sprite.Rotate(360.f*dt);
		else
			sprite.SetRotation(0);
		*/
		
		//checkcollisions();
	}
}

void Player::draw() {

	AnimatedActor::draw();
}

void Player::onColliding(Actor & otherActor)
{
	if(!dying) {
		if(otherActor.isEnemy())
		{
			doDamage(1.0f);
		}

		if(otherActor.isSpawnPoint()) {
			currentStart = static_cast<StartPoint *>(&otherActor);
		}

		if(otherActor.isExitPoint()) {
			currentStart = NULL;
			std::string mapname = static_cast<ExitPoint *>(&otherActor)->getMap();
			if(mapname == "credits_map.tmx") hidden = true;
			if(!mapname.empty()) {
				game_map->loadMap(mapname);
			}
		}
	}
}

void respawn()
{
	ui_showMenu();
	game_map->loadMap(game_map->currentFilename);
}

void Player::die() {
	if (godMode || dying)
		return;

    energy = 0;
	dieSound->playSound();
	lives--;
	dying = true;
	this->setCurrentAnimation("die");
	currentWeapon = 0;
}

void Player::onDestroy() {
	if (debugMode)
		cout << "player destroyed\n";
}

void Player::onAnimationComplete(std::string anim) {
	//cout << "EnemyWalker::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		if(lives > 0) {
			//std::cout << "life lost. current lives: " << lives << std::endl;

			// reset all collectibles
			for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
				if((*it)->isCollectible()) {
					static_cast<Collectible *>(*it)->reset();
				}
			}
			CollectibleEnergyBall * ball = new CollectibleEnergyBall(pos_x - 16, pos_y - 48);

			init();
		} else {
			lives = start_lives;
			energyBalls = 0;
			ui_popupImage("images/game_over.png", respawn);
		}
	}
}

bool Player::doDamage(float damage, bool knockback) {
	bool dead = false;
	if(damageTimer <= 0) {
		energy -= damage * 30;
		if(energy <= 0) {
			dead = true;
			die();
		} else {
				onDamage();
		}
		damageTimer = immunityTime;
		recoveryTimer = recoveryTime;

		if(knockback) {
			int knockback_direction = Facing::None;
			
			if(body->v.x < 0) {
				knockback_direction = Facing::Right;
			} else if(body->v.x > 0) {
				knockback_direction = Facing::Left;
			} else {
				if(facing_direction == Facing::Left) {
					knockback_direction = Facing::Right;
				} else {
					knockback_direction = Facing::Left;
				}
			}
			if(knockback_direction == Facing::Left) {
				body->v.x = -300;
			} else {
				body->v.x = 300;
			}
			body->v.y = 150;
		}
	}
  return dead;
}

void Player::onDamage() {
        soundCache["grunt.ogg"]->playSound();
}

void Player::addExperience(int exp) {
	cout << "Received " << exp << " experience\n";
	int oldLevel = currentLevel;
	currentExperience += exp;

	/*
	 * TODO: This is just a quick and dirty leveling system.  It should be easy
	 * enough to drop in a better replacement.
	 */
	const int exp_per_level = 3;
	int newLevel = currentExperience / exp_per_level + 1;
	while(currentLevel < newLevel) {
		incrementLevel();  // Calls onlevelUp
	}
}

void Player::onLevelUp(int newLevel) {
	cout << "Reached level " << newLevel << "!\n";
	// Play a sound and indicate a level up graphically
	energy_max = baseMaxEnergy + (newLevel - 1) * energyPerLevel;
}

int Player::getCurrentExperience() {
	return currentExperience;
}
