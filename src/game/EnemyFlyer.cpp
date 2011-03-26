#include "EnemyFlyer.h"
#include "Player.h"
#include "Map.h"

EnemyFlyer::EnemyFlyer()
:Enemy()
{
	
	fly_speed = 120.f;
	
	speed_x = 0;
	speed_y = 0;
	
	setPlaceholder(sf::Color(255, 0, 0), 16, 32, 0.5f, 1.0f);
}

EnemyFlyer::~EnemyFlyer()
{
}

void EnemyFlyer::update(float dt) {
	const float vision_range = 320;
	
	// Chase the player
	float dx = g_player->pos_x - pos_x;
	if (-vision_range < dx && dx < 0)
		speed_x = -fly_speed;
	if (0 < dx && dx < vision_range)
		speed_x = fly_speed;
	
	float delta_x = speed_x*dt;
	float delta_y = speed_y*dt;
	game_map->move(pos_x, pos_y, width, height, delta_x, delta_y);
	speed_x = delta_x / dt;
	speed_y = delta_y / dt;
	
	checkCollisions();
}


