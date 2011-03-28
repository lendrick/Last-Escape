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
// This class is for all actors with animations (or even still actors).  It inherits Actor.

/// Stores an AnimationFrame
///
struct Frame
{
	int number;
	sf::IntRect rect;
	float timeToNextFrame;
};

class Animation
{
public: 
	Animation(sf::Sprite&, std::string name = "");
	virtual ~Animation();
	void update(); //TODO may return if it finished
	void updateFrame(); //set's the sprite's subRect to the current Frame

	bool getIsFinished();
	void setIsFinished(bool);
	void setDoLoop(bool);
	void addFrame(Frame);

	/// Added for testing purposes.
	/// TODO read animations from a config file.
	/// This function loads the coords for the xeon idle animation
	void toDefaultXeonWalkAnimation();
	void toDefaultXeonJumpAnimation();
	void toDefaultXeonIdleAnimation();

	void setFrameSize(int fw, int fh);
	void addFrame(int num, float duration);
	void reset();
	
	int getFrame();
	std::string getName(); 
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
