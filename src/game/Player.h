#ifndef PLAYER_H
#define PLAYER_H

#include "Map.h"
#include "globals.h"

class Player {
private:

public:
	Player();
	~Player();
	
	void logic(float dt);
	void render();
	
	float pos_x;
	float pos_y;
	int width;
	int height;
	float speed_x;
	float speed_y;
	int state;
	sf::Image img;
	sf::Sprite spr;
};


#endif
