#pragma once
#include "globals.h"
// This class is for all actors with animations (or even still actors).  It inherits Actor.

/// Stores an AnimationFrame
///
struct Frame
{
	sf::IntRect rect;
	float timeToNextFrame;
};

class Animation
{
public: 
	Animation(sf::Sprite&);
	virtual ~Animation();
	void update();

	/// Added for testing purposes.
	/// TODO read animations from a config file.
	/// This function loads the coords for the xeon idle animation
	void toDefaultXeonAnimation();

private:
	vector<Frame> frames;		///< Contains the frames for the Animation.
	std::string name;
	sf::Sprite& sprite;			///< This sprite will be updated by the Animation.
	sf::Clock animationTimer;
	unsigned int frameIterator;	///< id to the current Frame.
	bool doLoop; 				///< true if the Animation shall be repeated.
};
