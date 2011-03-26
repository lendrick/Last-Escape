#pragma once
#include <globals.h>

class Actor {
public:
	Actor();
	~Actor();
	void setPos(float px, float py);
	void move(float mx, float my);
	void getPos(float &px, float &py);
	
	void setSize(int w, int h);
	void getSize(int &w, int &h);
	
	void setOrigin(int ox, int oy);
	void getOrigin(int &ox, int &oy);
	
	void getBoundingBox(int &x1, int &y1, int &x2, int &y2);
	
	bool isColliding(Actor * otherActor);
	
	virtual void update() = 0;
	virtual void draw() = 0;
	
	void destroy();
	bool isDestroyed();
	
private:
	int xOrigin, yOrigin;
	int height, width;
	float x, y;
	bool destroyed;
};