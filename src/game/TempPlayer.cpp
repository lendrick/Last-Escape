#include "TempPlayer.h"

TempPlayer::TempPlayer() {
	pos_x = 64.0f;
	pos_y = 400.0f;
	width = 24;
	height = 48;
	speed_x = 0.0f;
	speed_y = 0.0f;
	int state = 0;
	img.Create(24, 48, sf::Color(255, 255, 255));
}
	
void TempPlayer::logic() {

	const sf::Input& input = App->GetInput();
	const int speed_delta = 1.0;
	const int speed_max = 8.0;
	const int terminal_velocity = 16.0;
	
	// left/right move
	if (input.IsKeyDown(sf::Key::Left)) {
		speed_x -= speed_delta;
		if (speed_x < -speed_max) speed_x = -speed_max;
	}
	else if (input.IsKeyDown(sf::Key::Right)) {
		speed_x += speed_delta;
		if (speed_x > speed_max) speed_x = speed_max;
	}
	else {
		if (speed_x > speed_delta) speed_x -= speed_delta;
		else if (speed_x < -speed_delta) speed_x += speed_delta;
		else speed_x = 0.0;
	}
	
	// jumping
	if (input.IsKeyDown(sf::Key::Up)) {
		speed_y -= speed_delta;
		if (speed_y < -speed_max) speed_y = -speed_max; 
	}
	else { // gravity
		speed_y += speed_delta;
		if (speed_y > terminal_velocity) speed_y = terminal_velocity;
	}
	
	game_map->move(pos_x, pos_y, width, height, speed_x, speed_y);
}

void TempPlayer::render() {

	spr.SetPosition(pos_x - game_map->cam_x - width/2, pos_y - game_map->cam_y - height);
	spr.SetImage(img);
	App->Draw(spr);
}

TempPlayer::~TempPlayer() {

}

