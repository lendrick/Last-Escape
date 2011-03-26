/*
 *  Map.cpp
 *  Energy
 *
 *  Created by Clint Bellanger on 3/25/11.
 *  Copyright 2011 Clint Bellanger. All rights reserved.
 *
 */

#include "Map.h"

Map::Map(sf::RenderWindow &_target) {

	target = &_target;
	
	loadTileset("tileset.png");
	
	for (int i=0; i<VIEW_TILES_X; i++) {
		for (int j=0; j<VIEW_TILES_Y; j++) {
			sprites_bg[i][j].SetImage(tileset);
		}
	}
	
	// TEMP: random level
	for (int i=0; i<MAP_TILES_X; i++) {
		for (int j=0; j<MAP_TILES_Y; j++) {
			map_bg[i][j] = rand() % 5;
		}
	}
	
	// prep the rects for each tile
	for (int i=0; i<TILE_COUNT; i++) {
	
		// assumes tileset is 640px wide
		tile_rects[i].Left = (i % 20) * TILE_SIZE;
		tile_rects[i].Top = (i / 20) * TILE_SIZE;
		tile_rects[i].Right = tile_rects[i].Left + TILE_SIZE;
		tile_rects[i].Bottom = tile_rects[i].Top + TILE_SIZE;
	}
	
	cam_x = 0;
	cam_y = 0;
	
}

void Map::loadTileset(string filename) {

	tileset.LoadFromFile(("images/" + filename).c_str());
	
}

void Map::render() {

	int this_tile;
	
	// which tile is at the topleft corner of the screen?
	int cam_tile_x = cam_x / TILE_SIZE;
	int cam_tile_y = cam_y / TILE_SIZE;
	
	// how far offset is this tile (and each subsequent tiles)?
	int cam_off_x = cam_x % TILE_SIZE;
	int cam_off_y = cam_y % TILE_SIZE;

	// apply camera
	for (int i=0; i<VIEW_TILES_X; i++) {
		for (int j=0; j<VIEW_TILES_Y; j++) {
			
			sprites_bg[i][j].SetPosition(i*32 - cam_off_x, j*32 - cam_off_y);
			
			// and which sprite should we display?
			// outside the map
			if (cam_tile_x + i < 0 || cam_tile_x + i >= MAP_TILES_X ||
					cam_tile_y + j < 0 || cam_tile_y + j >= MAP_TILES_Y) {
				sprites_bg[i][j].SetSubRect(tile_rects[0]);
			}
			else { // inside the map
				this_tile = map_bg[cam_tile_x + i][cam_tile_y + j];
				sprites_bg[i][j].SetSubRect(tile_rects[this_tile]);
			}
		}
	}

	// render background
	for (int i=0; i<VIEW_TILES_X; i++) {
		for (int j=0; j<VIEW_TILES_Y; j++) {
			target->Draw(sprites_bg[i][j]);
		}
	}
}

Map::~Map() {

}
