#pragma once
#include "globals.h"
#include "Animation.h"
#include "Actor.h"
// This class is for all actors with animations (or even still actors).  It inherits Actor.

class AnimatedActor : public Actor
{
public:
	AnimatedActor(sf::Image&);
	AnimatedActor();
	void setImage(sf::Image&);
	
	virtual ~AnimatedActor();
	void collide(Actor*); 	///< implemented empty for testing;
	void update(float dt);	///< update Animation
	void setCurrentAnimation(std::string name); ///< Set's the current Animation for the given Name

protected:
	void flipDirection();
	int facing_direction;
	Animation *currentAnimation; ///< TODO use a queue
	std::map<std::string, Animation*> animations; ///< stores all available Animations for this Actor

private:
	void init();
};
