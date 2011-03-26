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
	void update(); //TODO may return if it finished
	void updateFrame(); //set's the sprite's subRect to the current Frame
	bool getIsFinished();
	void setIsFinished(bool);
	void setFrameSize(int fw, int fh);
	void addFrame(int num, float duration);
	void setLoop(bool loop);

	/// Added for testing purposes.
	/// TODO read animations from a config file.
	/// This function loads the coords for the xeon idle animation
	void toDefaultXeonWalkAnimation();
	void toDefaultXeonJumpAnimation();
	void toDefaultXeonIdleAnimation();

private:
	vector<Frame> frames;		///< Contains the frames for the Animation.
	std::string name;
	sf::Sprite& sprite;			///< This sprite will be updated by the Animation.
	sf::Clock animationTimer;   ///< Times the animation
	unsigned int frameIterator;	///< id to the current Frame.
	bool doLoop; 				///< true if the Animation shall be repeated.
	bool isFinished;			///< is true if the AnimationCycle finished.
	int frame_w, frame_h;
};
