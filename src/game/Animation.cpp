
#include "Animation.h"

Animation::Animation(sf::Sprite& sprite)
: sprite(sprite)
{
	this->animationTimer.Reset();
	this->frameIterator = 0;
}

Animation::~Animation()
{
}

// testing purposes
void Animation::toDefaultXeonAnimation()
{
	Frame f0;
	f0.rect = sf::IntRect(17,24, 68, 68);
	f0.timeToNextFrame = 0.3f;

	Frame f1;
	f1.rect = sf::IntRect(89,24, 68, 68);
	f1.timeToNextFrame = 0.3f;

	Frame f2;
	f2.rect = sf::IntRect(160,24, 68, 68);
	f2.timeToNextFrame = 0.4f;

	this->frames.push_back(f0);
	this->frames.push_back(f1);
	this->frames.push_back(f2);
	this->doLoop = true;
}

void Animation::update()
{
	if(!this->frames.empty() && animationTimer.GetElapsedTime() > this->frames.at(frameIterator).timeToNextFrame)
	{
		// is the last Frame in frames-vector.
		if(this->frames.size()-1 == frameIterator)
		{
			//Loop, restart Animation?
			if(this->doLoop)
			{
				frameIterator = 0;
				this->sprite.SetSubRect(frames.at(frameIterator).rect );
				cout << "looped" << endl;
			}
		}
		else
		{
			//Update to the next frame
			this->frameIterator++;
			this->sprite.SetSubRect(frames.at(frameIterator).rect );
		}
		animationTimer.Reset();
	}
}

