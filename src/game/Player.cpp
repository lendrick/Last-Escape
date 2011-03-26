#include "Player.h"

const int SPRITE_TILE_W = 128;
const int SPRITE_TILE_H = 128;

const int SPRITE_CENTER_X = 32;
const int SPRITE_CENTER_Y = 104;

const int SPRITE_IDLE_ROW = 0;
const int SPRITE_JUMP_ROW = 1;

Player::Player() {
	anim_time = 0.0f;
	pos_x = 64.0f;
	pos_y = 400.0f;
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

void Player::logic(float dt) {
	const sf::Input& input = App->GetInput();
	const int speed_max = 240; // pixels per second
	const int speed_delta = speed_max*4; // pixels per second per second
	const int speed_delta_decel = speed_max*4;
	const int terminal_velocity = 16.0*60;
	const int jump_speed = 380;
	
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
	
	// jumping
	if (input.IsKeyDown(sf::Key::Up)) {
		if (game_map->isGrounded(pos_x, pos_y, width))
			speed_y = -jump_speed;
	}
	
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
	if (!game_map->isGrounded(pos_x, pos_y, width) && speed_y < 0)
	{
		sprite_tile_row = SPRITE_JUMP_ROW;
	}
	else
	{
		sprite_tile_row = SPRITE_IDLE_ROW;
		if (speed_x != 0)
			sprite_tile_col = int(anim_time*8.f) % 4;
	}

	spr.SetSubRect(sf::IntRect(sprite_tile_col*SPRITE_TILE_W, sprite_tile_row*SPRITE_TILE_H,
		(sprite_tile_col+1)*SPRITE_TILE_W, (sprite_tile_row+1)*SPRITE_TILE_H));
	
	if (speed_x > 0)
	{
		spr.FlipX(false);
		spr.SetCenter(SPRITE_CENTER_X - width/2, SPRITE_CENTER_Y-height);
	}
	else if (speed_x < 0)
	{
		spr.FlipX(true);
		spr.SetCenter(SPRITE_TILE_W - SPRITE_CENTER_X - width/2, SPRITE_CENTER_Y-height);
	}
}

void Player::render() {

	spr.SetPosition(
		0.5f + (int)(pos_x - game_map->cam_x - width/2),
		0.5f + (int)(pos_y - game_map->cam_y - height));
	App->Draw(spr);
}


