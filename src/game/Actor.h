#pragma once
#include "globals.h"
class Player;

class Actor {
public:
	Actor();
	~Actor();

	void setPlaceholder(sf::Color c, float w, float h);

	void setPos(float px, float py);
	void move(float mx, float my);
	void getPos(float &px, float &py);
	
	void setSize(int w, int h);
	void getSize(int &w, int &h);
	
	void setOrigin(int ox, int oy);
	void getOrigin(int &ox, int &oy);
	
	void getBoundingBox(float &x1, float &y1, float &x2, float &y2);
	
	bool isColliding(Actor * otherActor);
	
	virtual void collide(Actor * otherActor) { }
	virtual void collidePlayer(Player& player) { }
	virtual void update(float dt) { }
	virtual void draw();
	
	void destroy();
	bool isDestroyed();
	
protected:
	sf::Image image;
	sf::Sprite sprite;

	int xOrigin, yOrigin;
	int height, width;
	float pos_x, pos_y;
	bool destroyed;
};
