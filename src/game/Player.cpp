#include "Player.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sound.h"
#include "globals.h"
#include "ImageCache.h"
#include "SoundCache.h"
#include "Ui.h"
#include "ExitPoint.h"

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
	{"Overcharged Blaster", 2.5, 0.1, 10.0,
		2, 3, 32.0 },
	{"Way Overcharged Blaster", 2.0, 0.01, 5.0,
		2, 3, 32.0 },
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
	
	loadAnimationsFromFile("xeon.xml");
	armor = 0;
	
	fireSound = soundCache["shoot.ogg"];
	currentStart = NULL;

	init();
}
	
void Player::init() {
	time = 0.f;
	last_shoot_time = -100.f;

	energy = energy_max = 100.f;
	
	if(currentStart == NULL)
		currentStart = findStart();
	
	float sx, sy;
	if(currentStart != NULL) {
		currentStart->getPos(sx, sy);
		setPos(sx, sy);
	} 
	
	// Find the first start point, and move the player there
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		if ((*it)->isStartPoint())
		{
			setPos((*it)->pos_x, (*it)->pos_y);
			break;
		}
	}

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
		if((*it)->isStartPoint()) {
			return static_cast<StartPoint *>(*it);
		}
	}
	
	return NULL;
}

void Player::upgradeWeapon() {
	current_weapon = min(current_weapon+1, num_weapon_types-1);
}

void Player::jump(float dt) {
	const int jump_speed = 380;
	const float max_jet_accel = 2000;
	const float jet_cost = 100;

	if (speed_y == 0 && isGrounded())
	{
		if (energy < energy_cost_jump)
			return;

		speed_y = -jump_speed;
		energy -= energy_cost_jump;

		last_jump_time = time;
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
	const int terminal_velocity = 16.0*60;
	walking = false;
	
	time += dt;

	if(isGrounded()) {
		speed_y = 0;
	}
	
	int move_direction = FACING_NONE; 

	// recharge energy
	energy += std::min(energy_recharge_rate*dt, std::max(0.f, energy_max - energy));

	if (godMode)
		energy = std::max(energy, 10.f);

	// left/right move
	if (input.direction() == FACING_LEFT) {
		move_direction = FACING_LEFT;
		speed_x -= speed_delta*dt;
		if (speed_x < -speed_max) speed_x = -speed_max;
		facing_direction = move_direction;
	}
	else if (input.direction() == FACING_RIGHT) {
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

	if (input.jumping())
		jump(dt);

	if (input.shooting())
		shoot();
	
	if(input.crouching())
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

	if(time - last_shoot_time < shoot_duration) {
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
	std:cout<< this->getCurrentAnimation()->getIsFinished();
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

void Player::draw() {

	AnimatedActor::draw();
}

void Player::collide(Actor & otherActor) 
{
	if(otherActor.isEnemy()) 
	{
		die();
	}
	
	if(otherActor.isSpawnPoint()) {
		currentStart = static_cast<StartPoint *>(&otherActor);
	}
	
	if(otherActor.isExitPoint()) {
		std::string mapname = static_cast<ExitPoint *>(&otherActor)->getMap();
		if(!mapname.empty()) {
			game_map->loadMap(mapname);
		}
		currentStart = NULL;
		init();
	}
	
	if (otherActor.isCollectible() || otherActor.isExitPoint())
	{
		otherActor.collide(*this);
	}
}

void bla()
{
	std::cout << "Bla" << std::endl;
	game_map->loadMap(game_map->currentFilename);
}

void Player::die() {
	if (godMode)
		return;

	if(armor == 0)
	{
		lifes--;
		if(lifes >= 0) {
			std::cout << "life lost. current lifes: " << lifes << std::endl;
			init();
		} else {
			lifes = start_lifes;
			ui_popupImage("images/game_over.png", bla);
		}
	}
}

void Player::onDestroy() {
	cout << "player destroyed\n";
}
