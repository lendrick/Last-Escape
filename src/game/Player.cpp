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

const double energy_cost_jump = 0.f;
const double energy_recharge_rate = 0.005f; // units per ms
static const int start_lives = 3;

/*struct WeaponDesc {
	const char* name;
	double energy_cost;
	double reload_time;
	double angle_variation;

	int sprite_row;
	int sprite_count;
	double sprite_speed;
};

const int num_weapon_types = 3;
WeaponDesc weapons[num_weapon_types] = {
	{"Blaster", 5.0, 0.5, 0.0,
		2, 3, 16.0 },
	{"Overcharged Blaster", 2.5f, 0.1f, 10.0f,
		2, 3, 32.0f },
	{"Way Overcharged Blaster", 2.0f, 0.01f, 5.0f,
		2, 3, 32.0f },
};*/

Weapon * currentWeapon;

Player::Player(double x, double y)
: AnimatedActor(x, y, 24, 48, "xeon.png", false) {

	lives = start_lives;
	setFrameSize(128, 128);
	setDrawOffset(64, 80);
	shoot_duration = 200;
	last_shoot_time = 0;
	last_jump_time = 0;
	energyBalls = 0;
	immunityTime = 1000;
	recoveryTime = 200;
	recoveryTimer = 0;
	//currentWeapon = 0;
	baseMaxEnergy = 100.0f;
	energyPerLevel = 10.0f;
	energy_max = baseMaxEnergy;
	currentExperience = 0;
	actorName = "Player";

	currentWeapon = new ChargerWeapon();

	loadAnimationsFromFile("xeon.xml");
	armor = 0;

	fireSound = soundCache["shoot.ogg"];
	jumpSound = soundCache["ambient_techno1.ogg"];
	dieSound  = soundCache["xeonDies.ogg"];
	currentStart = NULL;
	dying = false;
	canSleep = false;

	init();
}

void Player::init() {
	time = 0;
	last_shoot_time = -100;
	last_jump_time = 0;
	wasShooting = false;
	dying = false;


	energy = energy_max;

	if(currentStart == NULL)
		currentStart = findStart();

	double sx, sy;
	if(currentStart != NULL) {
		currentStart->getPos(sx, sy);
		//setPos(sx, sy);
	}

	std::cout << "Init player at " << sx << ", " << sy << std::endl;

	facing_direction = Facing::Right;
	crouched = false;

	this->setCurrentAnimation("idle");
	resetPhysics(sx, sy);
}

void Player::resetPhysicsCustom(double start_x, double start_y) {
	if(shape) {
		shape->layers = PhysicsLayer::Map|PhysicsLayer::Player|PhysicsLayer::Enemy|PhysicsLayer::EnemyBullet;
		shape->collision_type = PhysicsType::Player;
	}
}

StartPoint * Player::findStart() {
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		if((*it) && !(*it)->isDestroyed() && (*it)->isStartPoint()) {
			//if (debugMode)
				double px, py;
				(*it)->getPos(px, py);
				if(debugMode) cout << "found start at " << px << " " << py << "\n";
			return static_cast<StartPoint *>(*it);
		}
	}

	return NULL;
}

void Player::upgradeWeapon() {
	currentWeapon->upgradeWeapon();
}

void Player::jump(double dt) {
	const int jump_speed = 525;
	const double max_jet_accel = 2000;
	const double jet_cost = 35;
	const double jet_speed_max = 250;
	const double jet_wait = 400;
	const double jump_wait = 300;
	
	//cout << "jumping\n";

	if (isGrounded() && time - last_jump_time > jump_wait)
	{
		//cout << "jump!\n";
		if (energy < energy_cost_jump)
			return;

	  cpBodyApplyImpulse(body, cpv(0, 6000), cpv(0, 0));
		last_jump_time = time;
		jumpSound->playSound();
	}
	else
	{
		//if(isGrounded())
		//	cout << "too soon " << time - last_jump_time << "\n";
		//else 
		//	cout << "not grounded\n";
		
		// Can't jet immediately after jumping
		if (time - last_jump_time < jet_wait)
			return;

		double cost = jet_cost * dt/1000.f;
		if (energy < cost)
			return;

		energy -= cost;
		
		if(body->v.y < jet_speed_max)
			cpBodyApplyImpulse(body, cpv(0, 400), cpv(0, 0));
			
		if(jumpSound->getStatus() != sf::Sound::Playing)
			jumpSound->playSound();
	}
}


void Player::crouch() {
	crouched = true;
}

void Player::update(sf::Uint32 dt) {
	if(!body) return;
	const int speed_max = 200; // pixels per second
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
        energy += std::min(energy_recharge_rate*dt, std::max(0., energy_max - energy));

	if (godMode)
		energy = std::max(energy, 10.);

	// left/right move
	shape->u = 2.0f;
	if (!dying && recoveryTimer <= 0 && input.direction() == Facing::Left) {
		move_direction = Facing::Left;
		if(body->v.x > -speed_max)
			cpBodyApplyImpulse(body, cpv(-700, 0), cpv(0, 0));
		facing_direction = move_direction;
		walking = true;
		shape->u = 0.0f;
	}
	else if (!dying && recoveryTimer <= 0 && input.direction() == Facing::Right) {
		move_direction = Facing::Right;
		if(body->v.x < speed_max)
			cpBodyApplyImpulse(body, cpv(700, 0), cpv(0, 0));
		facing_direction = move_direction;
		walking = true;
		shape->u = 0.0f;
	}

	if (!dying && recoveryTimer <= 0 && input.jumping())
		jump(dt);
	else if(!dying && recoveryTimer <= 0 && !input.jumping() && body->v.y > 0) {
		cpBodyApplyImpulse(body, cpv(0, -220), cpv(0, 0));
	}

	if (!dying && recoveryTimer <= 0 && input.shooting())
	{
		if(!wasShooting)
			currentWeapon->startShooting(*this);
		currentWeapon->shooting(*this);
		wasShooting = true;
	}
	else if(!dying && recoveryTimer <= 0 && wasShooting)
	{
		currentWeapon->stopShooting(*this);
		wasShooting = false;
	}

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

		if(debugMode)
			cout << (isGrounded()?"grounded":"not-grounded") << " " << grounded << "\n";
		     
		updateSpriteFacing();

		/*
		if (speed_y == terminal_velocity)
			sprite.Rotate(360.f*dt);
		else
			sprite.SetRotation(0);
		*/
		
		//checkcollisions();
		//cout << body->p.x << " " << body->p.y << "\n";
	}
}

void Player::draw() {

	AnimatedActor::draw();
	currentWeapon->draw(*this);
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
			if(mapname == "credits") {
				gamestatus = Gamestatus::win;
				cout << "credits now" << endl;
			}
			if(!mapname.empty()) {
				game_map->setNextMap(mapname);
			}
		}
	}
}

void Player::die() {
	if (godMode || dying)
		return;

    energy = 0;
	dieSound->playSound();
	lives--;
	dying = true;
	this->setCurrentAnimation("die");
	//currentWeapon = 0;
	currentWeapon->reset();
}

void Player::onDestroy() {
	delete currentWeapon;
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
			CollectibleEnergyBall * ball = new CollectibleEnergyBall(body->p.x - 16, body->p.y);

			init();
		} else {
			lives = start_lives;
			energyBalls = 0;
			gamestatus = Gamestatus::gameOver;
		}
	}
}

bool Player::doDamage(double damage, bool knockback) {
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
				body->v.x = -200;
			} else {
				body->v.x = 200;
			}
			body->v.y = 300;
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

bool Player::hasKeycard(std::string name)
{
	for (list<std::string>::iterator it = keycards.begin();  it != keycards.end(); ++it)
	{
		if(*it == name)
			return true;
	}
	return false;
}
void Player::removeKeycard(std::string name)
{
	for (list<std::string>::iterator it = keycards.begin();  it != keycards.end(); ++it)
	{
		if(*it == name)
		{
			keycards.erase(it);
			return;
		}
	}
}
void Player::addKeycard(std::string name)
{
	keycards.push_back(name);
}
