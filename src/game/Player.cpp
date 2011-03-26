#include "Player.h"

#include "PlayerBullet.h"

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
const float energy_cost_shoot = 5.f;
const float energy_recharge_rate = 5.f; // units per second

Player::Player() {
	anim_time = 0.0f;
	last_shoot_time = -100.f;

	energy = energy_max = 100.f;

	pos_x = 64.0f;
	pos_y = 0.0f;
	facing_rightwards = true;
	width = 24;
	height = 48;
	speed_x = 0.0f;
	speed_y = 0.0f;
	int state = 0;
	if (!img.LoadFromFile("images/xeon.png"))
		printf("failed to load images/xeon.png\n");
	spr.SetImage(img);
	spr.SetCenter(SPRITE_CENTER_X - width/2, SPRITE_CENTER_Y-height);
}
	
Player::~Player() {

}

void Player::getBoundingBox(float &x1, float &y1, float &x2, float &y2) {
	x1 = pos_x - width/2;
	x2 = pos_x + width/2;
	y1 = pos_y - height;
	y2 = pos_y;
}

void Player::jump() {
	const int jump_speed = 380;

	if (energy < energy_cost_jump)
		return;

	if (speed_y == 0 && game_map->isGrounded(pos_x, pos_y, width))
	{
		speed_y = -jump_speed;
		energy -= energy_cost_jump;
	}
}

void Player::shoot() {
	const float shoot_reload_timer = 0.5f;
	
	if (energy < energy_cost_shoot)
		return;

	if (anim_time - last_shoot_time > shoot_reload_timer) {
		last_shoot_time = anim_time;
		energy -= energy_cost_shoot;

		Actor* bullet = new PlayerBullet(facing_rightwards);
		bullet->setPos(pos_x, pos_y - height/2);
	}
}

void Player::logic(float dt) {
	const sf::Input& input = App->GetInput();
	const int speed_max = 240; // pixels per second
	const int speed_delta = speed_max*4; // pixels per second per second
	const int speed_delta_decel = speed_max*4;
	const int terminal_velocity = 16.0*60;

	// recharge energy
	energy += std::min(energy_recharge_rate*dt, std::max(0.f, energy_max - energy));

	// left/right move
	if (input.IsKeyDown(sf::Key::Left)) {
		speed_x -= speed_delta*dt;
		if (speed_x < -speed_max) speed_x = -speed_max;
	}
	else if (input.IsKeyDown(sf::Key::Right)) {
		speed_x += speed_delta*dt;
		if (speed_x > speed_max) speed_x = speed_max;
	}
	else {
		if (speed_x > speed_delta_decel*dt) speed_x -= speed_delta_decel*dt;
		else if (speed_x < -speed_delta_decel*dt) speed_x += speed_delta_decel*dt;
		else speed_x = 0.0;
	}
	
	// various actions (main() already handles KeyPressed so it doesn't miss
	// any user inputs, but the extra KeyDown checks ensure it'll repeat properly
	// when held down)

	if (input.IsKeyDown(sf::Key::Up))
		jump();

	if (input.IsKeyDown(sf::Key::Space))
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

	anim_time += dt;

	int sprite_tile_col = 0, sprite_tile_row = 0;
	if (anim_time < last_shoot_time + SPRITE_SHOOT_COUNT/SPRITE_SHOOT_SPEED)
	{
		sprite_tile_row = SPRITE_SHOOT_ROW;
		sprite_tile_col = int((anim_time-last_shoot_time) * SPRITE_SHOOT_SPEED);
	}
	else if (!game_map->isGrounded(pos_x, pos_y, width) && speed_y < 0)
	{
		sprite_tile_row = SPRITE_JUMP_ROW;
	}
	else if (speed_x != 0)
	{
		sprite_tile_row = SPRITE_WALK_ROW;
		sprite_tile_col = int(anim_time*SPRITE_WALK_SPEED) % SPRITE_WALK_COUNT;
	}
	else
	{
		sprite_tile_row = SPRITE_IDLE_ROW;
		sprite_tile_col = 0;
	}

	spr.SetSubRect(sf::IntRect(sprite_tile_col*SPRITE_TILE_W, sprite_tile_row*SPRITE_TILE_H,
		(sprite_tile_col+1)*SPRITE_TILE_W, (sprite_tile_row+1)*SPRITE_TILE_H));
	
	if (speed_x > 0)
	{
		facing_rightwards = true;
		spr.FlipX(false);
		spr.SetCenter(SPRITE_CENTER_X - width/2, SPRITE_CENTER_Y-height);
	}
	else if (speed_x < 0)
	{
		facing_rightwards = false;
		spr.FlipX(true);
		spr.SetCenter(SPRITE_TILE_W - SPRITE_CENTER_X - width/2, SPRITE_CENTER_Y-height);
	}

	if (speed_y == terminal_velocity)
		spr.Rotate(360.f*dt);
	else
		spr.SetRotation(0);
}

void Player::render() {

	spr.SetPosition(
		0.5f + (int)(pos_x - game_map->cam_x - width/2),
		0.5f + (int)(pos_y - game_map->cam_y - height));
	App->Draw(spr);
}


