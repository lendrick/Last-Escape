/**
 *  Map
 */
 
#include "Map.h"

Map::Map(sf::RenderWindow &_target) {

	target = &_target;
	
	loadTileset("tileset.png");
	
	for (int i=0; i<VIEW_TILES_X; i++) {
		for (int j=0; j<VIEW_TILES_Y; j++) {
			tile_sprites[i][j].SetImage(tileset);
		}
	}
	
	// prep the rects for each tile
	for (int i=0; i<TILE_COUNT-1; i++) {
	
		// assumes tileset is 512px wide
		tile_rects[i+1].Left = (i % 16) * TILE_SIZE;
		tile_rects[i+1].Top = (i / 16) * TILE_SIZE;
		tile_rects[i+1].Right = tile_rects[i+1].Left + TILE_SIZE;
		tile_rects[i+1].Bottom = tile_rects[i+1].Top + TILE_SIZE;
	}
	
	cam_x = 0;
	cam_y = 0;
	
	loadMap("test_map.txt");
	
}

void Map::loadTileset(string filename) {

	tileset.LoadFromFile(("images/" + filename).c_str());
	
}


void Map::loadMap(string filename) {

	ifstream infile;
	string line;
	string starts_with;
	string section;
	string key;
	string val;
	string cur_layer;
	int width;
	int height;
	unsigned int comma;
	
	// clear level
	for (int i=0; i<MAP_TILES_X; i++) {
		for (int j=0; j<MAP_TILES_Y; j++) {
			background[i][j] = 0;
			foreground[i][j] = 0;
			fringe[i][j] = 0;
			collision[i][j] = false;
			
		}
	}
	
	infile.open(("maps/" + filename).c_str(), ios::in);

	if (infile.is_open()) {
		while (!infile.eof()) {

			getline(infile, line);
			line = trim(line, '\r');

			if (line.length() == 0) continue;
			starts_with = line.at(0);
			if (starts_with == "#") continue;
			else if (starts_with == "[") {
				section = trim(parse_section_title(line), ' ');
				
				// read in tile layers
				if (section == "background" || section == "fringe" || section == "foreground" || section == "collision") {
					for (int j=0; j<height; j++) {
						getline(infile, line);
						line = trim(line, '\r');
						line = line + ",";
						
						for (int i=0; i<width; i++) {
							comma = line.find_first_of(',');
							if (section == "background")
								background[i][j] = atoi(line.substr(0, comma).c_str());
							else if (section == "fringe")
								fringe[i][j] = atoi(line.substr(0, comma).c_str());
							else if (section == "foreground")
								foreground[i][j] = atoi(line.substr(0, comma).c_str());
							else if (section == "collision")
								if (line.substr(0, comma) == "1") collision[i][j] = true;

							line = line.substr(comma+1, line.length());
						}
					}
				}
			}
			else { // this is data.  treatment depends on section type
				parse_key_pair(line, key, val);          
				key = trim(key, ' ');
				val = trim(val, ' ');
				
				if (key == "width") width = atoi(val.c_str());
				else if (key == "height") height = atoi(val.c_str());
			}
		}
	}

}


void Map::render() {
	
	// which tile is at the topleft corner of the screen?
	int cam_tile_x = cam_x / TILE_SIZE;
	int cam_tile_y = cam_y / TILE_SIZE;
	
	// how far offset is this tile (and each subsequent tiles)?
	int cam_off_x = cam_x % TILE_SIZE;
	int cam_off_y = cam_y % TILE_SIZE;

	// apply camera
	for (int i=0; i<VIEW_TILES_X; i++) {
		for (int j=0; j<VIEW_TILES_Y; j++) {
			tile_sprites[i][j].SetPosition(i*32 - cam_off_x + 0.5f, j*32 - cam_off_y + 0.5f);
		}
	}

	// render background
	for (int i=0; i<VIEW_TILES_X; i++) {
		if (cam_tile_x + i < 0 || cam_tile_x + i >= MAP_TILES_X) continue;
		for (int j=0; j<VIEW_TILES_Y; j++) {
			if (cam_tile_y + j < 0 || cam_tile_y + j >= MAP_TILES_Y) continue;
			tile_sprites[i][j].SetSubRect(tile_rects[background[cam_tile_x + i][cam_tile_y + j]]);
			target->Draw(tile_sprites[i][j]);
		}
	}
	

	// render fringe
	for (int i=0; i<VIEW_TILES_X; i++) {
		if (cam_tile_x + i < 0 || cam_tile_x + i >= MAP_TILES_X) continue;
		for (int j=0; j<VIEW_TILES_Y; j++) {
			if (cam_tile_y + j < 0 || cam_tile_y + j >= MAP_TILES_Y) continue;
			tile_sprites[i][j].SetSubRect(tile_rects[fringe[cam_tile_x + i][cam_tile_y + j]]);
			target->Draw(tile_sprites[i][j]);
		}
	}

	// render foreground
	for (int i=0; i<VIEW_TILES_X; i++) {
		if (cam_tile_x + i < 0 || cam_tile_x + i >= MAP_TILES_X) continue;
		for (int j=0; j<VIEW_TILES_Y; j++) {
			if (cam_tile_y + j < 0 || cam_tile_y + j >= MAP_TILES_Y) continue;
			tile_sprites[i][j].SetSubRect(tile_rects[foreground[cam_tile_x + i][cam_tile_y + j]]);
			target->Draw(tile_sprites[i][j]);
		}
	}


}

Map::~Map() {

}
