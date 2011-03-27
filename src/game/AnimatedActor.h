#pragma once

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
	AnimatedActor(std::string filename);
	AnimatedActor();
	void setImage(std::string filename);
	
	virtual ~AnimatedActor();
	void collide(Actor*); 	///< implemented empty for testing;
	virtual void draw();	///< update Animation
	void loadAnimationsFromFile(std::string filepath);
	
	void setCurrentAnimation(std::string name, bool reset = true); ///< Set's the current Animation for the given Name
	void resetCurrentAnimation();
	std::string animationName();
	
	Animation * addAnimation(std::string name);
	void setFrameSize(int fw, int fh);
	void updateSpriteFacing();
	
	// This event function is called when a specific animation completes.
	// Use it, for instance, to destroy the actor after it dies.
	virtual void onAnimationComplete(std::string anim) {};
	
protected:
	void flipDirection();
	int facing_direction;
	Animation *currentAnimation; ///< TODO use a queue
	std::map<std::string, Animation*> animations; ///< stores all available Animations for this Actor
	int frame_h, frame_w;
	
private:
	void init();
};
