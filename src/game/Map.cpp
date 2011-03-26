/**
 *  Map
 */
 
#include "Map.h"

#include "Collectible.h"
#include "Enemy.h"

Map::Map() {
	
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
	
	loadMap("subwaymap.txt");

	// TODO: load these from a data file
	(new EnemyWalker())->setPos(384, 256);

	(new CollectiblePill())->setPos(128, 256);
	(new CollectiblePill())->setPos(128+32, 256-32);
	(new CollectiblePill())->setPos(128+64, 256-64);
	(new CollectiblePill())->setPos(128+96, 256-96);
	(new CollectiblePill())->setPos(800, 300);
	(new CollectiblePill())->setPos(820, 425);

	for (float y = 270; y < 360; y += 16)
		for (float x = 256; x < 768; x += 16)
			(new CollectiblePill())->setPos(x, y);
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
								collision[i][j] = atoi(line.substr(0, comma).c_str());

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

/**
 * check horizontal line for collision at any point
 */
bool Map::checkHorizontalLine(int x1, int x2, int y) {
	int check_x1 = x1 >> TILE_SHIFT;
	int check_x2 = x2 >> TILE_SHIFT;
	int check_y = y >> TILE_SHIFT;
	
	if (check_y < 0 || check_y >= MAP_TILES_Y) return false; // outside map
	for (int check_x=check_x1; check_x<=check_x2; check_x++) {
		if (check_x < 0 || check_x >= MAP_TILES_X) return false; // outside map
		if (collision[check_x][check_y]) return false; // solid tile
	}
	return true;	
}

/**
 * check vertical line for collision at any point
 */
bool Map::checkVerticalLine(int x, int y1, int y2) {
	int check_x = x >> TILE_SHIFT;
	int check_y1 = y1 >> TILE_SHIFT;
	int check_y2 = y2 >> TILE_SHIFT;
	
	if (check_x < 0 || check_x >= MAP_TILES_X) return false; // outside map
	for (int check_y=check_y1; check_y<=check_y2; check_y++) {
		if (check_y < 0 || check_y >= MAP_TILES_Y) return false; // outside map
		if (collision[check_x][check_y]) return false; // solid tile
	}
	return true;
}

/**
 * Attempt to move object at pos(x,y), of size(x,y), desired delta (x,y)
 * Assumes maximum move is tile size!
 * If unable to do so, move as much as possible and set the new pos(x,y)
 */
void Map::move(float &pos_x, float &pos_y, int size_x, int size_y, float &move_x, float &move_y) {
	float orig_x = pos_x;
	float orig_y = pos_y;
	int current_tile;
	float check_x;
	float check_y;
	
	// horizontal movement first
	if (move_x > 0.0) { // if moving right
	
		// start at top-right corner
		check_x = pos_x + size_x/2;
		check_y = pos_y - size_y;
		
		current_tile = (int)check_x >> TILE_SHIFT;
		
		check_x += move_x + 1;
		if (current_tile != (int)check_x >> TILE_SHIFT) {
		
			// crossed into a new tile, so check all points on this edge
			if (checkVerticalLine((int)check_x, (int)check_y, (int)check_y + size_y)) {
				pos_x = check_x - size_x/2;
			}
			else { // move to the tile edge
				pos_x = ((int)check_x >> TILE_SHIFT) * TILE_SIZE - size_x/2 - 1;
				move_x = pos_x - orig_x;
			}
		
		}
		else { // didn't cross into a new tile, so simply move
			pos_x = check_x - size_x/2; 
		}
	
	}
	else if (move_x < 0.0) { // if moving left
	
		// start at top-left corner
		check_x = pos_x - size_x/2;
		check_y = pos_y - size_y;
		
		current_tile = (int)check_x >> TILE_SHIFT;
		
		check_x += move_x - 1;
		if (current_tile != (int)check_x >> TILE_SHIFT) {
		
			// crossed into a new tile, so check all points on this edge
			if (checkVerticalLine((int)check_x, (int)check_y, (int)check_y + size_y)) {
				pos_x = check_x + size_x/2;
			}
			else { // move to the tile edge
				pos_x = (((int)check_x >> TILE_SHIFT)+1) * TILE_SIZE + size_x/2 + 1;
				move_x = pos_x - orig_x;
			}
		
		}
		else { // didn't cross into a new tile, so simply move
			pos_x = check_x + size_x/2; 
		}
	}
	
	// vertical movement second
	if (move_y > 0) { // if moving down

		// start at bottom-left corner
		check_x = pos_x - size_x/2;
		check_y = pos_y;
		
		current_tile = (int)check_y >> TILE_SHIFT;
		
		check_y += move_y + 1;
		if (current_tile != (int)check_y >> TILE_SHIFT) {
		
			// crossed into a new tile, so check all points on this edge
			if (checkHorizontalLine((int)check_x, (int)check_x + size_x, (int)check_y)) {
				pos_y = check_y;
			}
			else { // move to the tile edge
				pos_y = ((int)check_y >> TILE_SHIFT) * TILE_SIZE -1;
				move_y = pos_y - orig_y;
			}
		
		}
		else { // didn't cross into a new tile, so simply move
			pos_y = check_y; 
		}
	}
	else if (move_y < 0) { // if moving up
	
		// start at top-left corner
		check_x = pos_x - size_x/2;
		check_y = pos_y - size_y;
		
		current_tile = (int)check_y >> TILE_SHIFT;
		
		check_y += move_y;
		if (current_tile != (int)check_y >> TILE_SHIFT) {
		
			// crossed into a new tile, so check all points on this edge
			if (checkHorizontalLine((int)check_x, (int)check_x + size_x, (int)check_y)) {
				pos_y = check_y + size_y;
			}
			else { // move to the tile edge
				pos_y = (((int)check_y >> TILE_SHIFT)+1) * TILE_SIZE +1 + size_y;
				move_y = pos_y - orig_y;
			}
		
		}
		else { // didn't cross into a new tile, so simply move
			pos_y = check_y + size_y; 
		}
	}

	game_map->cam_x = (int)pos_x - 320;
	game_map->cam_y = (int)pos_y - 240;
	
}

// if there is collision directly underfoot, return true
bool Map::isGrounded(float &pos_x, float &pos_y, int size_x) {
	return !checkHorizontalLine((int)(pos_x-size_x/2), (int)(pos_x+size_x/2), pos_y+1);
}

void Map::renderBackground() {
	
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
			App->Draw(tile_sprites[i][j]);
		}
	}

}

// and fringe
void Map::renderForeground() {

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

	// render fringe
	for (int i=0; i<VIEW_TILES_X; i++) {
		if (cam_tile_x + i < 0 || cam_tile_x + i >= MAP_TILES_X) continue;
		for (int j=0; j<VIEW_TILES_Y; j++) {
			if (cam_tile_y + j < 0 || cam_tile_y + j >= MAP_TILES_Y) continue;
			tile_sprites[i][j].SetSubRect(tile_rects[fringe[cam_tile_x + i][cam_tile_y + j]]);
			App->Draw(tile_sprites[i][j]);
		}
	}

	// render foreground
	for (int i=0; i<VIEW_TILES_X; i++) {
		if (cam_tile_x + i < 0 || cam_tile_x + i >= MAP_TILES_X) continue;
		for (int j=0; j<VIEW_TILES_Y; j++) {
			if (cam_tile_y + j < 0 || cam_tile_y + j >= MAP_TILES_Y) continue;
			tile_sprites[i][j].SetSubRect(tile_rects[foreground[cam_tile_x + i][cam_tile_y + j]]);
			App->Draw(tile_sprites[i][j]);
		}
	}
}

Map::~Map() {

}
