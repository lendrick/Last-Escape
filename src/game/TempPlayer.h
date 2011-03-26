/**
 * Primitive rectangular player without all the OO, animations, etc.
 */
#ifndef TEMP_PLAYER_H
#define TEMP_PLAYER_H

#include "Map.h"
#include "globals.h"

class TempPlayer {
private:

public:
	TempPlayer();
	~TempPlayer();
	
	void logic();
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
