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

#include <chipmunk/chipmunk.h>

#include <fstream>
#include <sstream>

#include "globals.h"
#include "tinyxml/tinyxml.h"
#include "Animation.h"
#include "Actor.h"
// This class is for all actors with animations (or even still actors).  It inherits Actor.

class AnimatedActor : public Actor
{
public:
	AnimatedActor(float x, float y, std::string filename);
	AnimatedActor(float x, float y);
	void setImage(std::string filename);
	
	virtual ~AnimatedActor();
	void collide(Actor*); 	///< implemented empty for testing;
	virtual void draw();	///< update Animation
	void loadAnimationsFromFile(std::string filepath);
	
	void setCurrentAnimation(std::string name, bool reset = true); ///< Set's the current Animation for the given Name
	Animation* getCurrentAnimation();
	void resetCurrentAnimation();
	std::string animationName();
	
	Animation * addAnimation(std::string name);
	void setFrameSize(int fw, int fh);
	void updateSpriteFacing();
	
	// This event function is called when a specific animation completes.
	// Use it, for instance, to destroy the actor after it dies.
	virtual void onAnimationComplete(std::string anim) {};
	
	void setFacing(int direction);
	virtual bool doDamage(float damage);
	virtual void onDamage();
	virtual void doUpdate(float dt);
	
	virtual bool move(float dx, float dy);

	void resetPhysics();
	cpBody* body;
	cpShape* shape;
protected:
	void flipDirection();
	int facing_direction;
	Animation *currentAnimation; ///< TODO use a queue
	std::map<std::string, Animation*> animations; ///< stores all available Animations for this Actor
	int frame_h, frame_w;
        
        float life;
        float immunityTime;
	float damageTimer;
private:
	void init();
};
