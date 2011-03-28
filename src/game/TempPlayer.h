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
