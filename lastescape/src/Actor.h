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
#include <QtCore>


class Player;
class Bumper;

class Actor : public QObject
{
  Q_OBJECT
	Q_PROPERTY(bool canCollide READ canCollide WRITE setCanCollide)
	Q_PROPERTY(bool dying READ isDying WRITE setDying)
	Q_PROPERTY(bool grounded READ isGrounded)
public:
  Actor(double x, double y, double w, double h, bool staticBody = false);
  virtual ~Actor();

	QList<Bumper *> bumpers;
	cpBody* body;
	cpShape* shape;

	sf::Image image;
	sf::Sprite sprite;

	int height, width;
	bool destroyed;
	int xDrawOffset, yDrawOffset;
	bool collideable;
	bool dying;
	bool hasImage;
	bool hidden;
	bool toTeleport;
	bool awake;
	bool canSleep;

	int currentLevel;
	int experienceValue;
	std::string actorName;
	cpVect groundVelocity;

	int destroyedCount;
	// Event functions
	//TODO: rename collide onCollide
	virtual void collideCallback(Actor& otherActor) {
		(void)otherActor;
	};
	virtual void bumperCollideCallback(int facing) {
		(void)facing;
	};
	virtual void bumperCollidingCallback(int facing) {
		(void)facing;
	};
	virtual void bumperEndCollideCallback(int facing) {
		(void)facing;
	};
	virtual void collidingCallback(Actor & otherActor) {
		(void)otherActor;
	};
	virtual void endCollideCallback(Actor & otherActor) {
		(void)otherActor;
	};
	virtual void destroyCallback() { };
	virtual void damageCallback() { };

public slots:
	QScriptValue addBumper(int facing, double thickness = 1);
	void setPlaceholder(sf::Color c, double w, double h, double xoff = 0.5, double yoff = 0.5);

  void setDrawOffset(int ox, int oy);
  void getPos(double &px, double &py);

  void setSize(int w, int h);
  void getSize(int &w, int &h);

  //void setOrigin(int ox, int oy);
  //void getOrigin(int &ox, int &oy);

  //void getBoundingBox(double &x1, double &y1, double &x2, double &y2);

  //bool isColliding(Actor * otherActor);
  bool isGrounded();

  // This function takes care of things that happen when the actor dies, if applicable
  // Start death animation, etc.  destroy() should be called in update() and not in collide()
  virtual void die();

	virtual void updateCallback(double dt) {
    (void)dt;
  };
  virtual	void doUpdate(double dt);
  virtual void draw();

  void destroy();
  bool isDestroyed();
  bool canCollide();
  void setCanCollide(bool col);
  bool isDying();
	void setDying(bool d);
  void goToGround();

  // stupid version of dynamic casting
  virtual bool isPlayer() {
    return false;
  }
  virtual bool isEnemy() {
    return false;
  }
  virtual bool isCollectible() {
    return false;
  }
  virtual bool isStartPoint() {
    return false;
  }
  virtual bool isExitPoint() {
    return false;
  }
  virtual bool isSpawnPoint() {
    return false;
  }
  virtual bool isTeleportEnter() {
    return false;
  }
  virtual bool isTeleportExit() {
    return false;
  }

  virtual void setLevel(int newLevel);
  virtual int getLevel();
  virtual void incrementLevel();
  virtual void onLevelUp(int newLevel) {
    (void)newLevel;
  }

	virtual void collideGround(cpVect v);
  virtual void collideWall() {}

  //TODO  Get rid of this function.
  virtual void leaveGround();

  void teleport(double x, double y, double vx, double vy);
  void doTeleport();

  bool isStaticBody() {
    return staticBody;
  }

  int getExperienceValue();
  void setExperienceValue(int exp);
  cpLayers getShapeLayers() {
    return shapeLayers;
  }
  void setShapeLayers(cpLayers l);

  void setVelocityFunc(cpBodyVelocityFunc f);

  // Set the velocity function to ignore gravity, and set motion to zero
  void freeze();

  // Set the velocity function to the default
  void unFreeze();

  virtual void resetPhysics(double start_x, double start_y);
  virtual void destroyPhysics();
  virtual void resetPhysicsCustom(double start_x, double start_y) {
    (void)start_x;
    (void)start_y;
  }

  bool isOnCamera();

protected:
  //void //checkcollisions();
  cpBodyVelocityFunc defaultVelocityFunc;
  double static_x, static_y;
  double teleport_x, teleport_y, teleport_vx, teleport_vy;
  int grounded;
  bool staticBody;
  cpLayers shapeLayers;
	cpCollisionType collisionType;
	int collisionGroup;
};

void no_gravity(struct cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);
void no_gravity_stop(struct cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);
