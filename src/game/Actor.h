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

#pragma once
#include "globals.h"
#include <string>

#include <chipmunk/chipmunk.h>
#include <chipmunk/chipmunk_unsafe.h>


class Player;

class Actor {
public:
	Actor(float x, float y, float w, float h, bool staticBody = false);
	~Actor();
	
	void setPlaceholder(sf::Color c, float w, float h, float xoff = 0.5, float yoff = 0.5);

	void setPos(float px, float py);
	void setDrawOffset(int ox, int oy);
	
	// returns true if the actor collided with a map tile
	virtual bool move(float &mx, float &my);
	
	void getPos(float &px, float &py);
	
	void setSize(int w, int h);
	void getSize(int &w, int &h);
	
	//void setOrigin(int ox, int oy);
	//void getOrigin(int &ox, int &oy);
	
	//void getBoundingBox(float &x1, float &y1, float &x2, float &y2);
	
	//bool isColliding(Actor * otherActor);
	bool isGrounded();
	
	// Event functions
	//TODO: rename collide onCollide
	virtual void collide(Actor& otherActor) { };
	virtual void onColliding(Actor & otherActor) { };
	virtual void onEndCollide(Actor & otherActor) { };
	virtual void onDestroy() { };
	virtual void onDamage() { };
	
	// This function takes care of things that happen when the actor dies, if applicable
	// Start death animation, etc.  destroy() should be called in update() and not in collide()
	virtual void die();
	
	virtual void update(float dt) { };
        virtual void doUpdate(float dt);
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
	virtual bool isTeleportEnter() { return false; }
	virtual bool isTeleportExit() { return false; }
	
	virtual void setLevel(int newLevel);
	virtual int getLevel();
	virtual void incrementLevel();
	virtual void onLevelUp(int newLevel) {}
	bool isStaticBody() { return staticBody; }
	
	int getExperienceValue();
	void setExperienceValue(int exp);
	cpLayers getShapeLayers() { return shapeLayers; }
	void setShapeLayers(cpLayers l);
	
	void resetPhysics();
	void destroyPhysics();
	cpBody* body;
	cpShape* shape;


	sf::Image image;
	sf::Sprite sprite;

	int height, width;
	float pos_x, pos_y;
	bool destroyed;
	int xDrawOffset, yDrawOffset;
	bool collideable;
	bool dying;
	bool hasImage;
	bool hidden;
	
	int currentLevel;
	int experienceValue;
	std::string actorName;
	
protected:
	//void //checkcollisions();

private:
	bool staticBody;
	cpLayers shapeLayers;
};
