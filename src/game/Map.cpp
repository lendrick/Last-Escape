/**
 *  Map
 */
 
#include "Map.h"

#include "tinyxml/tinyxml.h"

#include "Collectible.h"
#include "Enemy.h"
#include "EnemyWalker.h"
#include "EnemyFlyer.h"
#include "Particles.h"
#include "StartPoint.h"
#include "ExitPoint.h"

Map::Map(const char* mapName) {
	
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
	cameraFollow = NULL;
	
	loadMap(mapName);
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
	
	TiXmlDocument doc;
	if (!doc.LoadFile(("maps/" + filename).c_str()))
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
			const char* defaultType;

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

				Actor* actor;
				if (type == "pill")
					actor = new CollectiblePill();
				else if (type == "weaponupgrade")
					actor = new CollectibleWeaponUpgrade();
				else if (type == "armor")
					actor = new CollectibleArmor();
				else if (type == "smoke")
					actor = new ParticleEmitter();
				else if (type == "walker")
					actor = new EnemyWalker();
				else if (type == "flyer")
					actor = new EnemyFlyer();
				else if (type == "start")
					actor = new StartPoint();
				else if (type == "exit")
					actor = new ExitPoint(w, h);
				else
				{
					printf("unrecognised object type %s\n", type.c_str());
					continue;
				}
				int x = 0, y = 0;
				((TiXmlElement*)object)->QueryIntAttribute("x", &x);
				((TiXmlElement*)object)->QueryIntAttribute("y", &y);
				actor->setPos(x, y);
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
				}
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


bool Map::move(Actor &actor, float &move_x, float &move_y) {
	return move(actor.pos_x, actor.pos_y, actor.width, actor.height, move_x, move_y);
}

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
				pos_x = (((int)check_x >> TILE_SHIFT)+1) * TILE_SIZE + size_x/2 + 1;
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
		if (current_tile != (int)check_y >> TILE_SHIFT) {
		
			// crossed into a new tile, so check all points on this edge
			if (checkHorizontalLine((int)check_x, (int)check_x + size_x, (int)check_y)) {
				pos_y = check_y;
			}
			else { // move to the tile edge
				pos_y = ((int)check_y >> TILE_SHIFT) * TILE_SIZE -1;
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
		if (current_tile != (int)check_y >> TILE_SHIFT) {
		
			// crossed into a new tile, so check all points on this edge
			if (checkHorizontalLine((int)check_x, (int)check_x + size_x, (int)check_y)) {
				pos_y = check_y + size_y;
			}
			else { // move to the tile edge
				pos_y = (((int)check_y >> TILE_SHIFT)+1) * TILE_SIZE +1 + size_y;
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
	return !checkHorizontalLine((int)(pos_x-size_x/2), (int)(pos_x+size_x/2), pos_y+1);
}

void Map::renderBackground() {
	if(cameraFollow != NULL) {
		game_map->cam_x = (int)cameraFollow->pos_x - 320;
		game_map->cam_y = (int)cameraFollow->pos_y - 240;
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
			tile_sprites[i][j].SetSubRect(tile_rects[background[cam_tile_x + i][cam_tile_y + j]]);
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

Map::~Map() {

}
