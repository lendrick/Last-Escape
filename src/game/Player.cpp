#include "Player.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sound.h"
#include "globals.h"

const int SPRITE_TILE_W = 128;
const int SPRITE_TILE_H = 128;

const int SPRITE_CENTER_X = 32;
const int SPRITE_CENTER_Y = 104;

const int SPRITE_IDLE_ROW = 0;

const int SPRITE_WALK_ROW = 0;
const int SPRITE_WALK_COUNT = 4;
const float SPRITE_WALK_SPEED = 8.f;

const int SPRITE_JUMP_ROW = 1;

const int SPRITE_SHOOT_ROW = 2;
const int SPRITE_SHOOT_COUNT = 3;
const float SPRITE_SHOOT_SPEED = 16.f;

const float energy_cost_jump = 10.f;
const float energy_recharge_rate = 5.f; // units per second

struct WeaponDesc {
	const char* name;
	float energy_cost;
	float reload_time;
	float angle_variation;

	int sprite_row;
	int sprite_count;
	float sprite_speed;
};

WeaponDesc weapons[] = {
	{"Blaster", 5.0, 0.5, 0.0,
		2, 3, 16.0 },
	{"Overcharged Blaster", 2.5, 0.1, 10.0,
		2, 3, 32.0 },
};

Player::Player()
: AnimatedActor() {
	if (!image.LoadFromFile("images/xeon.png"))
		printf("failed to load images/xeon.png\n");
	this->setImage(image);
	
	width = 24;
	height = 48;
	xOrigin = width/2;
	yOrigin = height;
	setDrawOffset(64, 104);
	setFrameSize(128, 128);
	shoot_duration = 0.6f;
	last_shoot_time = 0;
	
	//sprite.SetCenter(SPRITE_CENTER_X - xOrigin, SPRITE_CENTER_Y - yOrigin);
	
	/*
	Animation* walkAnimation = new Animation(this->sprite);
	walkAnimation->toDefaultXeonWalkAnimation();
	this->animations["walk"] = walkAnimation;
	
	Animation* jumpAnimation = new Animation(this->sprite);
	jumpAnimation->toDefaultXeonJumpAnimation();
	this->animations["jump"] = jumpAnimation;
	
	Animation* idleAnimation = new Animation(this->sprite);
	idleAnimation->toDefaultXeonIdleAnimation();
	this->animations["idle"] = idleAnimation;
	*/
	
	Animation * tmp;
	
	tmp = addAnimation("walk");
	tmp->addFrame(0, .2f);
	tmp->addFrame(1, .2f);
	tmp->addFrame(2, .2f);
	tmp->addFrame(3, .2f);
	tmp->setLoop(true);
	
	tmp = addAnimation("jump");
	tmp->addFrame(24, 0.1f);
	tmp->addFrame(25, 0.1f);
	tmp->addFrame(26, 0.1f);
	
	tmp = addAnimation("fall");
	tmp->addFrame(27, 0.2f);
	tmp->addFrame(28, 0.1f);
	
	tmp = addAnimation("idle");
	tmp->addFrame(0, 0.2f);
	
	tmp = addAnimation("shoot");
	tmp->addFrame(16, 0.1f);
	tmp->addFrame(17, 0.1f);
	tmp->addFrame(18, 0.1f);

	init();
}
	
Player::~Player() {

}

void Player::init() {
	time = 0.f;
	last_shoot_time = -100.f;
	
	energy = energy_max = 100.f;
	
	pos_x = 64.0f;
	pos_y = 0.0f;
	facing_direction = FACING_RIGHT;

	speed_x = 0.0f;
	speed_y = 0.0f;

	// Set Animations


	this->setCurrentAnimation("idle");

	current_weapon = 0;
}

void Player::jump() {
	const int jump_speed = 380;

	if (energy < energy_cost_jump)
		return;

	if (speed_y == 0 && isGrounded())
	{
		speed_y = -jump_speed;
		energy -= energy_cost_jump;
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
		bullet->setPos(pos_x, pos_y - height/2);
  	setCurrentAnimation("shoot");
		resetCurrentAnimation();
	}
}

void Player::update(float dt) {
	const int speed_max = 240; // pixels per second
	const int speed_delta = speed_max*4; // pixels per second per second
	const int speed_delta_decel = speed_max*4;
	const int terminal_velocity = 16.0*60;
	
	time += dt;

	if(isGrounded()) {
		speed_y = 0;
	}
	
	int move_direction = FACING_NONE; 

	// recharge energy
	energy += std::min(energy_recharge_rate*dt, std::max(0.f, energy_max - energy));
	
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
	

	
	// various actions (main() already handles KeyPressed so it doesn't miss
	// any user inputs, but the extra KeyDown checks ensure it'll repeat properly
	// when held down)

	if (input.jumping())
		jump();

	if (input.shooting())
		shoot();

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
		this->setCurrentAnimation("shoot");
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
		this->setCurrentAnimation("walk");
	}
	else
	{
		this->setCurrentAnimation("idle"); 
	}

	if (facing_direction == FACING_RIGHT)
	{
		sprite.FlipX(false);
		//sprite.SetCenter(SPRITE_CENTER_X, SPRITE_CENTER_Y);
	}
	else if (facing_direction == FACING_LEFT)
	{
		sprite.FlipX(true);
		//sprite.SetCenter(SPRITE_TILE_W - SPRITE_CENTER_X, SPRITE_CENTER_Y);
	}

	if (speed_y == terminal_velocity)
		sprite.Rotate(360.f*dt);
	else
		sprite.SetRotation(0);
	
	checkCollisions();
}

void Player::draw() {
	/*
	sprite.SetPosition(
		0.5f + (int)(pos_x - game_map->cam_x - width/2),
		0.5f + (int)(pos_y - game_map->cam_y - height));
//	App->Draw(sprite);
*/
	AnimatedActor::draw();
}

void Player::collide(Actor & otherActor) 
{
	if(otherActor.isEnemy()) 
	{
		die();
	}
	
	if (otherActor.isCollectible())
	{
		otherActor.collide(*this);
	}
}

void Player::die() {
	init();
}
