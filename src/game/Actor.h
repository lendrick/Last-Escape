#pragma once
#include "globals.h"
class Player;

class Actor {
public:
	Actor();

	void setPlaceholder(sf::Color c, float w, float h, float xoff = 0.5, float yoff = 0.5);

	void setPos(float px, float py);
	void setDrawOffset(int ox, int oy);
	
	// returns true if the actor collided with a map tile
	bool move(float &mx, float &my);
	
	void getPos(float &px, float &py);
	
	void setSize(int w, int h);
	void getSize(int &w, int &h);
	
	void setOrigin(int ox, int oy);
	void getOrigin(int &ox, int &oy);
	
	void getBoundingBox(float &x1, float &y1, float &x2, float &y2);
	
	bool isColliding(Actor * otherActor);
	bool isGrounded();
	
	// Event functions
	//TODO: rename collide onCollide
	virtual void collide(Actor& otherActor) { }
	virtual void onDestroy() { };
	virtual void onDamage() { };
	
	// This function takes care of things that happen when the actor dies, if applicable
	// Start death animation, etc.  destroy() should be called in update() and not in collide()
	virtual void die();
	
	virtual void update(float dt) { };
	virtual void draw();
	
	void destroy();
	bool isDestroyed();
	bool canCollide();
	void setCanCollide(bool col);
	bool isDying();
	void goToGround();

	// stupid version of dynamic casting
	virtual bool isPlayer() { return false; }
	virtual bool isEnemy() { return false; }
	virtual bool isCollectible() { return false; }
	virtual bool isStartPoint() { return false; }
	virtual bool isExitPoint() { return false; }
	virtual bool isSpawnPoint() { return false; }

	sf::Image image;
	sf::Sprite sprite;

	int xOrigin, yOrigin;
	int height, width;
	float pos_x, pos_y;
	bool destroyed;
	int xDrawOffset, yDrawOffset;
	bool collideable;
	bool dying;
	~Actor();
protected:
	void checkCollisions();
};
