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

#include "Map.h"

#include "tinyxml/tinyxml.h"

#include "Collectible.h"
#include "Enemy.h"
#include "EnemyWalker.h"
#include "EnemyCrawler.h"
#include "EnemyFlyer.h"
#include "EnemyCentipede.h"
#include "BossSpider.h"
#include "Particles.h"
#include "Player.h"
#include "Teleport.h"
#include "StartPoint.h"
#include "SpawnPoint.h"
#include "ExitPoint.h"
#include "Player.h"
#include "Actor.h"
#include <cstdlib>

Map::Map(string mapName) {

	physSpace = NULL;

	loadTileset("tileset.png");
	loaded = false;

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
	cameraFollow = NULL;

	loadMap(mapName);
}

void Map::loadTileset(string filename) {

	tileset.LoadFromFile(("images/" + filename).c_str());
	tileset.SetSmooth(false);
}


void Map::loadMap(string filename) {
	loaded = false;
	ifstream infile;
	string line;
	string starts_with;
	string section;
	string key;
	string val;
	string cur_layer;
	int width;
	int height;
	//unsigned int comma; // Unreferenced local variable

	currentFilename = filename;

	for (int i=0; i<MAP_TILES_X; i++) {
		for (int j=0; j<MAP_TILES_Y; j++) {
			background[i][j] = 0;
			foreground[i][j] = 0;
			fringe[i][j] = 0;
			collision[i][j] = false;
		}
	}
	clear();

	TiXmlDocument doc;

	if(filename == "") {
		return;
	}
	else if (!doc.LoadFile(("maps/" + filename).c_str()))
	{
		printf("failed to open map\n");
		return;
	}

	TiXmlElement* root = doc.RootElement();
	root->QueryIntAttribute("width", &width);
	root->QueryIntAttribute("height", &height);

	for (TiXmlNode* child = root->FirstChild(); child; child = child->NextSibling())
	{
		std::string childName = child->Value();
		if (childName == "layer")
		{
			typedef int tiles_t[MAP_TILES_X][MAP_TILES_Y];
			tiles_t* tiles;

			std::string layerName = ((TiXmlElement*)child)->Attribute("name");
			if (layerName == "background")
				tiles = &background;
			else if (layerName == "fringe")
				tiles = &fringe;
			else if (layerName == "foreground")
				tiles = &foreground;
			else if (layerName == "collision")
				tiles = &collision;
			else if (layerName == "danger")
				tiles = &danger;
			else
				continue;

			const char* text = ((TiXmlElement*)child->FirstChild())->GetText();
			if (!text)
				continue;

			const char* start = text;
			for (int j = 0; j < height; j++) {
				for (int i = 0; i < width; i++) {
					const char* end = strchr(start, ',');
					if (!end)
						end = start + strlen(start);
					(*tiles)[i][j] = atoi(std::string(start, end).c_str());
					start = end+1;
				}
			}
		}
		else if (childName == "objectgroup")
		{
			const char* defaultType = NULL;

			// Look for a 'type' property as default for the whole group
			TiXmlElement* prop = TiXmlHandle(child).FirstChild("properties").FirstChild("property").ToElement();
			if (prop && strcmp(prop->Attribute("name"), "type") == 0)
				defaultType = prop->Attribute("value");

			for (TiXmlNode* object = child->FirstChild(); object; object = object->NextSibling())
			{
				// Skip everything except <object>
				if (strcmp(object->Value(), "object") != 0)
					continue;

				std::string type;
				if (((TiXmlElement*)object)->Attribute("type"))
				{
					type = ((TiXmlElement*)object)->Attribute("type");
				}
				else
				{
					if (defaultType)
						type = defaultType;
					else
						continue;
				}

				int w = 0, h = 0;
				((TiXmlElement*)object)->QueryIntAttribute("width", &w);
				((TiXmlElement*)object)->QueryIntAttribute("height", &h);
				const char* name = ((TiXmlElement*)object)->Attribute("name");

				int x = 0, y = 0;
				((TiXmlElement*)object)->QueryIntAttribute("x", &x);
				((TiXmlElement*)object)->QueryIntAttribute("y", &y);

				Actor* actor;
				if (type == "pill") {
					actor = new CollectiblePill((float)x, (float)y);
				} else if (type == "weaponupgrade") {
					actor = new CollectibleWeaponUpgrade((float)x, (float)y);
				} else if (type == "armor") {
					actor = new CollectibleArmor((float)x, (float)y);
				} else if (type == "smoke") {
					actor = new ParticleEmitter((float)x, (float)y);
				} else if (type == "walker") {
					actor = new EnemyWalker((float)x, (float)y);
				} else if (type == "crawler") {
					actor = new EnemyCrawler((float)x, (float)y);
				} else if (type == "flyer") {
					actor = new EnemyFlyer((float)x, (float)y);
				} else if (type == "centipede") {
					actor = new EnemyCentipede((float)x, (float)y);
				} else if (type == "spider") {
					actor = new BossSpider((float)x, (float)y);
				} else if (type == "teleportenter") {
					actor = new TeleportEnter((float)x, (float)y, w, h, name);
				} else if (type == "teleportexit") {
					actor = new TeleportExit((float)x, (float)y, name);
				} else if (type == "start") {
					actor = new StartPoint((float)x, (float)y);
				} else if (type == "spawn") {
					actor = new SpawnPoint((float)x, (float)y);
				} else if (type == "exit") {
					actor = new ExitPoint((float)x, (float)y, w, h);
					if (debugMode)
						cout << "Exit point\n";
					TiXmlElement* prop = TiXmlHandle(object).FirstChild("properties").FirstChild("property").ToElement();

					if(prop != NULL) {
						std::string mapname;
						std::string attrname = ((TiXmlElement*)prop)->Attribute("name");
						if(attrname == "map")
							mapname = ((TiXmlElement*)prop)->Attribute("value");
						if (!mapname.empty())
							dynamic_cast<ExitPoint *>(actor)->setMap(mapname);
					}
				} else {
					printf("unrecognised object type %s\n", type.c_str());
					continue;
				}
			}
		}
		else if(childName == "properties")
		{
			for (TiXmlNode* prop = child->FirstChild(); prop; prop = prop->NextSibling())
			{
				std::string propname = ((TiXmlElement*)prop)->Attribute("name");
				std::string propval = ((TiXmlElement*)prop)->Attribute("value");
				if(propname == "landscape") {
					landscapeImg.LoadFromFile("images/landscapes/" + propval);
					landscapeImg.SetSmooth(false);
					landscape.SetImage(landscapeImg);
				} else if(propname == "music" && enableMusic) {
                                        backgroundMusic.Stop();
                                        backgroundMusic.OpenFromFile("audio/" + propval);
					backgroundMusic.SetLoop(true);
					backgroundMusic.Play();
				}
			}
		}
	}

	this->setupPhysics();

	if(g_player != NULL) {
		g_player->init();
		cameraFollow = g_player;
	}
	loaded = true;
}

/*
 * TODO: "Trace" polygons around map for physics engine, instead of having a 
 * bunch of individual squares
 */

bool Map::setupPhysics()
{
	// Possibly reset the physics.
	if(physSpace) {
		cpSpaceFreeChildren(physSpace);
		cpSpaceFree(physSpace);
	}
	cpResetShapeIdCounter();

	physSpace = cpSpaceNew();
	physSpace->iterations = 10;
	physSpace->damping = 0.9f;
	physSpace->gravity = cpv(0, -1500);

	// Ah, come on...
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		Actor * actor = *it;
		if(actor->isPlayer()) {
			Player* p = dynamic_cast<Player*>(actor);
			p->resetPhysics();
		}
	}


	// Create a physics collision box for each collision tile.
	// If PROFILING SHOWS that this is slow, try to replace them by "floor lines".
	
	// Vertical Pass
	/* This only accounts for on and off collision tiles now, but would be easy
	 * enough to expand to working with diagonals as well, if we do a diagonal
	 * pass and account for multiple tile types.
	 */
	for (int i=0; i<MAP_TILES_X - 1; i++) {
		bool prev_different = false;
		cpVect p1, p2;
		
		for (int j=0; j<MAP_TILES_Y; j++) {
			if(collision[i][j] == collision[i+1][j]) {
				if(prev_different) {
					p2 = sfml2cp(sf::Vector2f(TILE_SIZE * (i + 1), TILE_SIZE * j));
					prev_different = false;
					cpShape * seg = cpSegmentShapeNew(&physSpace->staticBody, p1, p2, 0);
					seg->e = 0.0f;
					seg->u = 1.0f;
					cpSpaceAddShape(physSpace, seg);
				}
			} else {
				if(!prev_different) {
					p1 = sfml2cp(sf::Vector2f(TILE_SIZE * (i + 1), TILE_SIZE * j));
					prev_different = true;
				}
			}
		}
		
		if(prev_different) {
			p2 = sfml2cp(sf::Vector2f(TILE_SIZE * (i + 1), TILE_SIZE * MAP_TILES_Y));
			cpShape * seg = cpSegmentShapeNew(&physSpace->staticBody, p1, p2, 1);
			seg->e = 0.0f;
			seg->u = 1.0f;
			cpSpaceAddShape(physSpace, seg);
		}
		prev_different = false;
	}
	
	for (int j=0; j<MAP_TILES_Y - 1; j++) {
		bool prev_different = false;
		cpVect p1, p2;
		for (int i=0; i<MAP_TILES_X; i++) {	
			if(collision[i][j] == collision[i][j + 1]) {
				if(prev_different) {
					p2 = sfml2cp(sf::Vector2f(TILE_SIZE * i, TILE_SIZE * (j + 1)));
					prev_different = false;
					cpShape * seg = cpSegmentShapeNew(&physSpace->staticBody, p1, p2, 0);
					seg->e = 0.0f;
					seg->u = 1.0f;
					cpSpaceAddShape(physSpace, seg);
				}
			} else {
				if(!prev_different) {
					p1 = sfml2cp(sf::Vector2f(TILE_SIZE * i, TILE_SIZE * (j + 1)));
					prev_different = true;
				}
			}
		}
			
		if(prev_different) {
			p2 = sfml2cp(sf::Vector2f(TILE_SIZE * MAP_TILES_X, TILE_SIZE * (j + 1)));
			cpShape * seg = cpSegmentShapeNew(&physSpace->staticBody, p1, p2, 1);
			seg->e = 0.0f;
			seg->u = 1.0f;
			cpSpaceAddShape(physSpace, seg);
		}
		prev_different = false;
	}

			/*
			int current_tile = collision[i][j];
			int above = 0;
			int left = 0;
			
			if(i > 0)
				left = collision[i-1][j];
			
			if(j > 0)
				above = collision[i][j-1];
			
			cpVect topleft = sfml2cp(sf::Vector2f(TILE_SIZE * i, TILE_SIZE * j));
			cpVect topright = sfml2cp(sf::Vector2f(TILE_SIZE * (i + 1), TILE_SIZE * j));
			cpVect bottomleft = sfml2cp(sf::Vector2f(TILE_SIZE * i, TILE_SIZE  * (j + 1)));
			
			if((current_tile == 0 && above != 0) || 
				 (current_tile != 0 && above == 0))
			{
				cpShape * seg = cpSegmentShapeNew(&physSpace->staticBody, topleft, topright, 1);
				seg->e = 0.0f;
				seg->u = 1.0f;
				cpSpaceAddShape(physSpace, seg);
			}
				
			if((current_tile == 0 && left != 0) || 
				 (current_tile != 0 && left == 0))
			{
				cpShape * seg = cpSegmentShapeNew(&physSpace->staticBody, topleft, bottomleft, 1);
				seg->e = 0.0f;
				seg->u = 1.0f;
				cpSpaceAddShape(physSpace, seg);				
			}
			*/
			
			/*
			if(collision[i][j]) {
				// Tile box around 0,0
				cpVect verts[] = {
					cpv(-TILE_SIZE/2.0f, -TILE_SIZE/2.0f),
					cpv(-TILE_SIZE/2.0f,  TILE_SIZE/2.0f),
					cpv( TILE_SIZE/2.0f,  TILE_SIZE/2.0f),
					cpv( TILE_SIZE/2.0f, -TILE_SIZE/2.0f),
				};
				
				// Move to center of the tile.
				sf::Vector2f sfTileCenter = sf::Vector2f(TILE_SIZE*i + TILE_SIZE/2.0f, TILE_SIZE*j + TILE_SIZE/2.0f);
				cpVect offs = sfml2cp(sfTileCenter);
				cpShape *shape = cpSpaceAddShape(physSpace, cpPolyShapeNew(&physSpace->staticBody, 4, verts, offs));
				shape->e = 0.0f; shape->u = 1.0f;
// 				shape->layers = PhysLayer::MapGround;
// 				shape->collision_type = PhysType::MapFloor;
				std::cout << "Added block at SFML " << sfTileCenter.x << ", " << sfTileCenter.y << " that's cp " << offs.x << ", " << offs.y << std::endl;
			}
			*/
		//}
	//}

	return true;
}

bool Map::isLoaded() {
	return loaded;
}

// Convert SFML 0,0 = top left csys and y down
// to cp 0,0 = bottom left csys and y up.
cpVect Map::sfml2cp(const sf::Vector2f& v) const
{
	return cpv(v.x, MAP_TILES_Y*TILE_SIZE - v.y);
}

// Convert cp 0,0 = bottom left csys and y up
// to SFML 0,0 = top left csys and y down.
sf::Vector2f Map::cp2sfml(const cpVect& v) const
{
	return sf::Vector2f(v.x, MAP_TILES_Y*TILE_SIZE - v.y);
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


/*
 * TODO: REMOVE THIS FUNCTION AND RELY ON PHYSICS ENGINE FOR MOVEMENT
 */

bool Map::move(Actor &actor, float &move_x, float &move_y) {
	return move(actor.pos_x, actor.pos_y, actor.width, actor.height, move_x, move_y);
}

/*
 * TODO: REMOVE THIS FUNCTION AND RELY ON PHYSICS ENGINE FOR MOVEMENT
 */

/**
 * Attempt to move object at pos(x,y), of size(x,y), desired delta (x,y)
 * Assumes maximum move is tile size!
 * If unable to do so, move as much as possible and set the new pos(x,y)
 */
bool Map::move(float &pos_x, float &pos_y, int size_x, int size_y, float &move_x, float &move_y) {
	float orig_x = pos_x;
	float orig_y = pos_y;
	int current_tile;
	float check_x;
	float check_y;
	bool impact = false;

	// prevent falling through obstacles when going too fast due to
	// abnormally low framerates
	move_x = max(min(move_x, (float)TILE_SIZE), -(float)TILE_SIZE);
	move_y = max(min(move_y, (float)TILE_SIZE), -(float)TILE_SIZE);

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
				pos_x = float(((int)check_x >> TILE_SHIFT) * (float)TILE_SIZE - size_x / 2.0f - 1.0f);
				move_x = pos_x - orig_x;
				impact = true;
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
				pos_x = float((((int)check_x >> TILE_SHIFT)+1) * (float)TILE_SIZE + size_x / 2.0f + 1.0f);
				move_x = pos_x - orig_x;
				impact = true;
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
#define NO_MANUAL_GROUND_COLLISION
#ifdef NO_MANUAL_GROUND_COLLISION
		if (false) {
#else
		if (current_tile != (int)check_y >> TILE_SHIFT) {
#endif

			// crossed into a new tile, so check all points on this edge
			if (checkHorizontalLine((int)check_x, (int)check_x + size_x, (int)check_y)) {
				pos_y = check_y;
			}
			else { // move to the tile edge
				pos_y = float(((int)check_y >> TILE_SHIFT) * TILE_SIZE - 1);
				move_y = pos_y - orig_y;
				impact = true;
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
#define NO_MANUAL_GROUND_COLLISION
#ifdef NO_MANUAL_GROUND_COLLISION
		if (false) {
#else
		if (current_tile != (int)check_y >> TILE_SHIFT) {
#endif

			// crossed into a new tile, so check all points on this edge
			if (checkHorizontalLine((int)check_x, (int)check_x + size_x, (int)check_y)) {
				pos_y = check_y + size_y;
			}
			else { // move to the tile edge
				pos_y = float((((int)check_y >> TILE_SHIFT)+1) * TILE_SIZE + 1 + size_y);
				move_y = pos_y - orig_y;
				impact = true;
			}

		}
		else { // didn't cross into a new tile, so simply move
			pos_y = check_y + size_y;
		}
	}

	return impact;
}

void Map::setCameraFollow(Actor * actor) {
	cameraFollow = actor;
}

bool Map::isGrounded(Actor & actor) {
	return isGrounded(actor.pos_x, actor.pos_y, actor.width);
}

// if there is collision directly underfoot, return true
bool Map::isGrounded(float &pos_x, float &pos_y, int size_x) {
	return !checkHorizontalLine((int)(pos_x-size_x/2), (int)(pos_x+size_x/2), int(pos_y+1.0f));
}

bool Map::isOnInstantdeath(Actor &actor)
{
	return danger[(int)actor.pos_x/TILE_SIZE][(int)actor.pos_y/TILE_SIZE] != 0;
}

bool Map::isSolid(int x, int y) {
	return collision[x/TILE_SIZE][y/TILE_SIZE] != 0;
}

void Map::renderBackground() {
	if(cameraFollow != NULL) {
		game_map->cam_x = std::max(0, (int)cameraFollow->pos_x - (int)App->GetWidth()/2);
		game_map->cam_y = std::max(0, (int)cameraFollow->pos_y - (int)App->GetHeight()/2);
	}

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
			if (!background[cam_tile_x + i][cam_tile_y + j]) continue;
			tile_sprites[i][j].SetSubRect(tile_rects[background[cam_tile_x + i][cam_tile_y + j]]);
			App->Draw(tile_sprites[i][j]);
		}
	}

	// render fringe
	for (int i=0; i<VIEW_TILES_X; i++) {
		if (cam_tile_x + i < 0 || cam_tile_x + i >= MAP_TILES_X) continue;
		for (int j=0; j<VIEW_TILES_Y; j++) {
			if (cam_tile_y + j < 0 || cam_tile_y + j >= MAP_TILES_Y) continue;
			if (!fringe[cam_tile_x + i][cam_tile_y + j]) continue;
			tile_sprites[i][j].SetSubRect(tile_rects[fringe[cam_tile_x + i][cam_tile_y + j]]);
			App->Draw(tile_sprites[i][j]);
		}
	}

}

void Map::actorDestroyed(Actor * actor) {
	if(actor == cameraFollow) {
		cameraFollow = NULL;
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

	// render foreground
	for (int i=0; i<VIEW_TILES_X; i++) {
		if (cam_tile_x + i < 0 || cam_tile_x + i >= MAP_TILES_X) continue;
		for (int j=0; j<VIEW_TILES_Y; j++) {
			if (cam_tile_y + j < 0 || cam_tile_y + j >= MAP_TILES_Y) continue;
			if (!foreground[cam_tile_x + i][cam_tile_y + j]) continue;
			tile_sprites[i][j].SetSubRect(tile_rects[foreground[cam_tile_x + i][cam_tile_y + j]]);
			App->Draw(tile_sprites[i][j]);
		}
	}
}

void Map::renderLandscape() {
	// Do nothing if no landscape was specified
	if (!landscapeImg.GetWidth())
		return;

	// Draw it four times, aka repeating in X and Y
	sf::Vector2f topleft(-(float)(cam_x/10 % landscapeImg.GetWidth()), -(float)(cam_y/10 % landscapeImg.GetHeight()));
	landscape.SetPosition(topleft);
	App->Draw(landscape);
	landscape.SetPosition(topleft.x + landscapeImg.GetWidth(), topleft.y);
	App->Draw(landscape);
	landscape.SetPosition(topleft.x, topleft.y + landscapeImg.GetHeight());
	App->Draw(landscape);
	landscape.SetPosition(topleft.x + landscapeImg.GetWidth(), topleft.y + landscapeImg.GetHeight());
	App->Draw(landscape);
}

void Map::clear() {
	for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		Actor * actor = *it;
		if(!actor->isPlayer()) actor->destroy();
	}
}

Map::~Map() {
	clear();
}
