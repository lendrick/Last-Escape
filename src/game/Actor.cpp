#include "Actor.h"
#include "Map.h"
#include "globals.h"
#include <list>

Actor::Actor() {
	setPos(0, 0);
	destroyed = false;
	actors.push_back(this);
}

Actor::~Actor() {
}

void Actor::setPlaceholder(sf::Color c, float w, float h) {
	width = w;
	height = h;
	xOrigin = w/2;
	yOrigin = h/2;
	sprite.SetColor(c);
	sprite.SetScale(width, height);
	sprite.SetCenter(0.5, 0.5);
}

void Actor::setPos(float px, float py) {
	pos_x = px;
	pos_y = py;
}

void Actor::move(float mx, float my) {
	pos_x += mx;
	pos_y += my;
}

void Actor::getPos(float &px, float &py) {
	px = pos_x;
	py = pos_y;
}

void Actor::setSize(int w, int h) {
	height = h;
	width = w;
}

void Actor::getSize(int &w, int &h) {
	h = height;
	w = width;
}

void Actor::setOrigin(int ox, int oy) {
	xOrigin = ox;
	yOrigin = oy;
}

void Actor::getOrigin(int &ox, int &oy) {
	ox = xOrigin;
	oy = yOrigin;
}

void Actor::getBoundingBox(float &x1, float &y1, float &x2, float &y2) {
	x1 = pos_x - xOrigin;
	y1 = pos_y - yOrigin;
	x2 = x1 + width;
	y2 = y1 + height;
}

bool Actor::isColliding(Actor * otherActor) {
	float x1, y1, x2, y2;
	float ox1, oy1, ox2, oy2;
	getBoundingBox(x1, y1, x2, y2);
	otherActor->getBoundingBox(ox1, oy1, ox2, oy2);
	
	if( ( (x1 < ox1 && ox1 < x2) || (x1 < ox2 && ox2 < x2) ) &&
		( (y1 < oy1 && oy1 < y2) || (y1 < oy2 && oy2 < y2) ) ) {
		return true;
	}
	
	return false;
}

void Actor::draw() {
	sprite.SetPosition(pos_x - game_map->cam_x, pos_y - game_map->cam_y);
	App->Draw(sprite);
}

void Actor::destroy() {
	destroyed = true;
}

bool Actor::isDestroyed() {
	return destroyed;
}
