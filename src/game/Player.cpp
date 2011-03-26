#include "Player.h"

Player::Player() {
	pos_x = 64.0f;
	pos_y = 400.0f;
	width = 24;
	height = 48;
	speed_x = 0.0f;
	speed_y = 0.0f;
	int state = 0;
	img.Create(24, 48, sf::Color(255, 255, 255));
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
}

void Player::render() {

	spr.SetPosition((int)(pos_x - game_map->cam_x - width/2), (int)(pos_y - game_map->cam_y - height));
	spr.SetImage(img);
	App->Draw(spr);
}


