#include "Actor.h"

Actor::Actor() {
	setPos(0, 0);
	destroyed = false;
}

Actor::~Actor() {
}

void Actor::setPos(float px, float py) {
	px = x;
	py = y;
}

void Actor::move(float mx, float my) {
	x += mx;
	y += my;
}

void Actor::getPos(float &px, float &py) {
	px = x;
	py = y;
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

void Actor::getBoundingBox(int &x1, int &y1, int &x2, int &y2) {
	x1 = x - xOrigin;
	y1 = y - yOrigin;
	x2 = x1 + width;
	y2 = y1 + height;
}

bool Actor::isColliding(Actor * otherActor) {
	int x1, y1, x2, y2;
	int ox1, oy1, ox2, oy2;
	getBoundingBox(x1, y1, x2, y2);
	otherActor->getBoundingBox(ox1, oy1, ox2, oy2);
	
	if( ( (x1 < ox1 && ox1 < x2) || (x1 < ox2 && ox2 < x2) ) &&
		( (y1 < oy1 && oy1 < y2) || (y1 < oy2 && oy2 < y2) ) ) {
		return true;
	}
	
	return false;
}

void Actor::destroy() {
	destroyed = true;
}

bool Actor::isDestroyed() {
	return destroyed;
}
