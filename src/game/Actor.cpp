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

void Actor::setPlaceholder(sf::Color c, float w, float h, float xoff, float yoff) {
	width = w;
	height = h;
	xOrigin = w*xoff;
	yOrigin = h*yoff;
	sprite.SetColor(c);
	sprite.SetScale(width, height);
	sprite.SetCenter(xoff, yoff);
}

void Actor::setPos(float px, float py) {
	pos_x = px;
	pos_y = py;
}

// returns true if the actor collided with a map tile
bool Actor::move(float mx, float my) {
  return game_map->move(*this, mx, my);
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
	
	if (x2 < ox1 || ox2 < x1 || y2 < oy1 || oy2 < y1)
		return false;
	return true;
}

void Actor::draw() {
	sprite.SetPosition(pos_x - game_map->cam_x, pos_y - game_map->cam_y);
	App->Draw(sprite);
}

void Actor::destroy() {
	onDestroy();
	game_map->actorDestroyed(this);
	destroyed = true;
}

bool Actor::isDestroyed() {
	return destroyed;
}

void Actor::checkCollisions() {
	list<Actor*>::iterator it2 = actors.begin();
	
	for (; it2 != actors.end(); ++it2)
	{
		// Don't collide with self. :)
		if (*it2 != this && !isDestroyed() && !(*it2)->isDestroyed() && isColliding(*it2))
		{
			collide(**it2);
		}
	}
}
