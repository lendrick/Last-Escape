/**
 *  Map
 */

#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;


// 32x32 tile size at 640x480.  One extra for scrolling
const int TILE_SIZE = 32;
const int VIEW_TILES_X = 21;
const int VIEW_TILES_Y = 16;
const int MAP_TILES_X = 1024;
const int MAP_TILES_Y = 256;
const int TILE_COUNT = 1024;

class Map {
private:
	sf::RenderWindow *target;
	sf::Image tileset;
	sf::IntRect tile_rects[1024];
	int map_bg[MAP_TILES_X][MAP_TILES_Y];
	bool map_coll[MAP_TILES_X][MAP_TILES_Y];
public:
	Map(sf::RenderWindow &_target);
	~Map();

	void loadTileset(string filename);
	void render();


	sf::Sprite sprites_bg[VIEW_TILES_X][VIEW_TILES_X];
	
	int cam_x;
	int cam_y;
};

#endif

