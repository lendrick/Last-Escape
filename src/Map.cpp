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

	// apply camera
	for (int i=0; i<VIEW_TILES_X; i++) {
		for (int j=0; j<VIEW_TILES_Y; j++) {
		
			sprites_bg[i][j].SetPosition(i*32,j*32);
			this_tile = map_bg[i][j];
			sprites_bg[i][j].SetSubRect(tile_rects[this_tile]);
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
