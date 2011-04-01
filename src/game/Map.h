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
 *  Map
 */

#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <string>
#include <fstream>
#include "Utils.h"
#include "globals.h"

using namespace std;

// 32x32 tile size at 640x480.  One extra for scrolling
const int TILE_SIZE = 32;
const int TILE_SHIFT = 5; // for fast divides and multiplies
const int VIEW_TILES_X = 21;
const int VIEW_TILES_Y = 16;
const int MAP_TILES_X = 1024;
const int MAP_TILES_Y = 256;
const int TILE_COUNT = 1024;

class Map {
private:
	sf::RenderWindow *target;
	sf::Image tileset;
	sf::Image landscapeImg;
	sf::Sprite landscape;
	sf::IntRect tile_rects[1024];
	
	// map layers
	int background[MAP_TILES_X][MAP_TILES_Y];
	int fringe[MAP_TILES_X][MAP_TILES_Y];
	int foreground[MAP_TILES_X][MAP_TILES_Y];
	int danger[MAP_TILES_X][MAP_TILES_Y];
	
	Actor * cameraFollow;

protected:
	void initPhysics();
	bool setupPhysics();
	
public:
	Map(string mapName);
	~Map();

	void loadMap(string filename);
	void loadTileset(string filename);
	void setNextMap(string filename);
	void loadNextMap();
	bool checkHorizontalLine(int x1, int x2, int y);
	bool checkVerticalLine(int x, int y1, int y2);
	void setCameraFollow(Actor * actor);
	bool move(float &pos_x, float &pos_y, int size_x, int size_y, float &move_x, float &move_y);
	bool move(Actor &actor, float &move_x, float &move_y);
	bool isGrounded(Actor &actor);
	bool isGrounded(float &pos_x, float &pos_y, int size_x);
	bool isOnInstantdeath(Actor &actor);
	bool isSolid(int x, int y);
	void renderLandscape();
	void renderBackground();
	void renderForeground();
	void clear();
	bool isLoaded();
	
	void createSegment(cpVect &p1, cpVect &p2, bool ground);

	cpVect sfml2cp(const sf::Vector2f& v) const;
	sf::Vector2f cp2sfml(const cpVect& v) const;

	void actorDestroyed(Actor * actor);

	// collision layer
	int collision[MAP_TILES_X][MAP_TILES_Y];

	sf::Sprite tile_sprites[VIEW_TILES_X][VIEW_TILES_X];
	
	int cam_x;
	int cam_y;
	bool loaded;

	std::string currentFilename;
	std::string nextMap;
	

	// Physics of the map.
	cpSpace *physSpace;
};


static int map_begin_collide(cpArbiter *arb, cpSpace *space, void *data);
static int map_colliding(cpArbiter *arb, cpSpace *space, void *data);
static void map_end_collide(cpArbiter *arb, cpSpace *space, void *data);

static int map_begin_ground_collide(cpArbiter *arb, cpSpace *space, void *data);
static void map_end_ground_collide(cpArbiter *arb, cpSpace *space, void *data);

static int map_bumper_begin_collide(cpArbiter *arb, cpSpace *space, void *data);
static int map_bumper_colliding(cpArbiter *arb, cpSpace *space, void *data);
static void map_bumper_end_collide(cpArbiter *arb, cpSpace *space, void *data);

static int map_bumper_begin_ground_collide(cpArbiter *arb, cpSpace *space, void *data);
static void map_bumper_end_ground_collide(cpArbiter *arb, cpSpace *space, void *data);

#endif

