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
static const int start_lifes = 3;

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

Player::Player()
: AnimatedActor() {
	setImage("xeon.png");

	lifes = start_lifes;
	width = 24;
	height = 48;
	xOrigin = width/2;
	yOrigin = height;
	setDrawOffset(64, 104);
	setFrameSize(128, 128);
	shoot_duration = .2f;
	last_shoot_time = 0;
	energyBalls = 0;
	immunityTime = 0.5f;
	recoveryTime = 0.2f;
	recoveryTimer = 0;

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

	energy = energy_max = 100.f;

	if(currentStart == NULL)
		currentStart = findStart();

	float sx, sy;
	if(currentStart != NULL) {
		currentStart->getPos(sx, sy);
		setPos(sx, sy);
	}

	/*
	// Find the first start point, and move the player there
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		if ((*it)->isStartPoint())
		{
			setPos((*it)->pos_x, (*it)->pos_y);
			break;
		}
	}
	*/

	facing_direction = FACING_RIGHT;
	crouched = false;

	speed_x = 0.0f;
	speed_y = 0.0f;

	// Set Animations
	goToGround();

	this->setCurrentAnimation("idle");

	current_weapon = 0;
}

StartPoint * Player::findStart() {
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		if((*it)->isStartPoint() && !(*it)->isDestroyed()) {
			cout << "found start at " << (*it)->pos_x << " " << (*it)->pos_y << "\n";
			return static_cast<StartPoint *>(*it);
		}
	}

	return NULL;
}

void Player::upgradeWeapon() {
	current_weapon = min(current_weapon+1, num_weapon_types-1);
}

void Player::jump(float dt) {
	const int jump_speed = 500;
	const float max_jet_accel = 2000;
	const float jet_cost = 100;

	if (speed_y == 0 && isGrounded())
	{
		if (energy < energy_cost_jump)
			return;

		speed_y = -jump_speed;
		energy -= energy_cost_jump;

		last_jump_time = time;
		jumpSound->playSound();
	}
	else
	{
		// Can't jet immediately after jumping
		if (time - last_jump_time < 0.5f)
			return;

		float cost = jet_cost * dt;
		if (energy < cost)
			return;

		energy -= cost;
		speed_y -= max_jet_accel*dt;
		if(jumpSound->getStatus() != sf::Sound::Playing)
			jumpSound->playSound();
	}
}

void Player::shoot() {
	const float shoot_reload_timer = 0.5f;

	if (energy < weapons[current_weapon].energy_cost)
		return;

	if (time - last_shoot_time > weapons[current_weapon].reload_time) {
		last_shoot_time = time;
		fireSound->playSound();
		energy -= weapons[current_weapon].energy_cost;

		Actor* bullet = new PlayerBullet(facing_direction, weapons[current_weapon].angle_variation);
		if(crouched) {
			bullet->setPos(pos_x, pos_y - 15);
		}
		else {
			bullet->setPos(pos_x, pos_y - 30);
		}


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
		lifes++;
		soundCache["1up.ogg"]->playSound();
	}

	time += dt;

	if(isGrounded() && recoveryTimer <= 0) {
		speed_y = 0;
	}

	if(game_map->isOnInstantdeath(*this)) {
		this->die();
	}

	int move_direction = FACING_NONE;

	// recharge energy
	energy += std::min(energy_recharge_rate*dt, std::max(0.f, energy_max - energy));

	if (godMode)
		energy = std::max(energy, 10.f);

	if(!dying && recoveryTimer <= 0 && input.stopJump() && !isGrounded() && speed_y < -50) {
		speed_y = -50;
	}
	// left/right move
	if (!dying && recoveryTimer <= 0 && input.direction() == FACING_LEFT) {
		move_direction = FACING_LEFT;
		speed_x -= speed_delta*dt;
		if (speed_x < -speed_max) speed_x = -speed_max;
		facing_direction = move_direction;
	}
	else if (!dying && recoveryTimer <= 0 && input.direction() == FACING_RIGHT) {
		move_direction = FACING_RIGHT;
		speed_x += speed_delta*dt;
		if (speed_x > speed_max) speed_x = speed_max;
		facing_direction = move_direction;
	}
	else {
		if (speed_x > speed_delta_decel*dt) speed_x -= speed_delta_decel*dt;
		else if (speed_x < -speed_delta_decel*dt) speed_x += speed_delta_decel*dt;
		else speed_x = 0.0;
	}

	if(speed_x !=0) {
		walking = true;
	}

	if (!dying && recoveryTimer <= 0 && input.jumping())
		jump(dt);

	if (!dying && recoveryTimer <= 0 && input.shooting())
		shoot();
	
	if(!dying && recoveryTimer <= 0 && input.crouching())
		crouched = true;
	else
		crouched = false;

	// gravity
	speed_y += speed_delta*dt;
	if (speed_y > terminal_velocity) speed_y = terminal_velocity;

	float delta_x = speed_x*dt;
	float delta_y = speed_y*dt;
	game_map->move(pos_x, pos_y, width, height, delta_x, delta_y);
	speed_x = delta_x / dt;
	speed_y = delta_y / dt;

	// Compute animations:

	if(!dying) {
                if(recoveryTimer > 0) {
                  this->setCurrentAnimation("damage");
                } if(time - last_shoot_time < shoot_duration) {
			if(walking) {
				this->setCurrentAnimation("walkshoot");
			}
			else if(crouched) {
				this->setCurrentAnimation("crouchshoot");
			}
			else {
				this->setCurrentAnimation("shoot");
			}
		}
		else if (!isGrounded() && speed_y != 0)
		{
			if(speed_y < 0)
				this->setCurrentAnimation("jump");
			else
				this->setCurrentAnimation("fall");
		}
		else if (speed_x != 0)
		{
			walking = true;
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

		updateSpriteFacing();

		if (speed_y == terminal_velocity)
			sprite.Rotate(360.f*dt);
		else
			sprite.SetRotation(0);

		checkCollisions();
	}
}

void Player::draw() {

	AnimatedActor::draw();
}

void Player::collide(Actor & otherActor)
{
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

	if (otherActor.isCollectible() || otherActor.isExitPoint() || otherActor.isTeleportEnter())
	{
		otherActor.collide(*this);
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

	if(armor == 0)
	{
		dieSound->playSound();
		lifes--;
		dying = true;
		this->setCurrentAnimation("die");
	}
}

void Player::onDestroy() {
	cout << "player destroyed\n";
}

void Player::onAnimationComplete(std::string anim) {
	//cout << "EnemyWalker::onAnimationComplete(\"" << anim << "\")\n";
	if(anim == "die") {
		if(lifes > 0) {
			//std::cout << "life lost. current lifes: " << lifes << std::endl;
			
			// reset all collectibles
			for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
				if((*it)->isCollectible()) {
					static_cast<Collectible *>(*it)->reset();
				}
			}
			init();
		} else {
			lifes = start_lifes;
			energyBalls = 0;
			ui_popupImage("images/game_over.png", respawn);
		}
	}
}

void Player::doDamage(float damage) {
        if(damageTimer <= 0) {
                energy -= damage * 30;
                if(energy < 0) {
                        die();
                } else {
                        onDamage();
                }
                damageTimer = immunityTime;
                recoveryTimer = recoveryTime;
                
                if(facing_direction == FACING_LEFT) {
                        speed_x = 300;
                } else {
                        speed_x = -300;
                }
                
                speed_y = -150;
        }
}

void Player::onDamage() {
        soundCache["grunt.ogg"]->playSound();
}
