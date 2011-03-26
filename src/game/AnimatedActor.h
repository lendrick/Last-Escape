#pragma once
#include "globals.h"
#include "Animation.h"
#include "Actor.h"
// This class is for all actors with animations (or even still actors).  It inherits Actor.

class AnimatedActor : public Actor
{
public:
	AnimatedActor(sf::Image&);
	virtual ~AnimatedActor();
	void collide(Actor*); 	///< implemented empty for testing;
	void update();  		///< update Animation
	void draw();    		///< draw Current AnimationFrame

private:
	Animation *currentAnimation; ///< TODO use a queue
	std::map<std::string, Animation*> animations; ///< stores all available Animations for this Actor
	
	//Drawing resources may move into a simper Actor subclass that implements simple drawing
	sf::Image image;
	sf::Sprite sprite;
};
