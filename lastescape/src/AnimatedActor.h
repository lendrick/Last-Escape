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

#include <fstream>
#include <sstream>

#include "globals.h"
#include "tinyxml/tinyxml.h"
#include "Animation.h"
#include "Actor.h"

#include <QtCore>

// This class is for all actors with animations (or even still actors).  It inherits Actor.

class AnimatedActor : public Actor
{
  Q_OBJECT
public:
	AnimatedActor(double x, double y, double w, double h, std::string filename, bool staticBody = false);
	AnimatedActor(double x, double y, double w, double h, bool staticBody = false);
	AnimatedActor(double x, double y, std::string filename);
	void setImage(std::string filename);

  virtual ~AnimatedActor();
	//void collide(Actor*); 	///< implemented empty for testing;
	virtual void draw();	///< update Animation
	void loadAnimationsFromFile(std::string filepath);
	void setCurrentAnimation(std::string name, bool reset = true);

	virtual void damageCallback();
	virtual void doUpdate(double dt);

public slots:
	virtual bool doDamage(double damage, bool knockback = false);

  Animation* getCurrentAnimation();
  void resetCurrentAnimation();
	std::string animationName();

	Animation * addAnimation(std::string name);
	void setFrameSize(int fw, int fh);

  // This event function is called when a specific animation completes.
  // Use it, for instance, to destroy the actor after it dies.
	virtual void animationCompleteCallback(std::string anim) {
    (void)anim;
  };

  void setFacing(int direction);
	void updateSpriteFacing();

protected:
  void flipDirection();
  int facing_direction;
  Animation *currentAnimation; ///< TODO use a queue
	std::map<std::string, Animation*> animations; ///< stores all available Animations for this Actor
  int frame_h, frame_w;

  double life;
  double immunityTime;
  double damageTimer;
private:
  void init();
};
