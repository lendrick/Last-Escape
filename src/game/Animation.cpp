
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
	f0.rect = sf::IntRect(10,40, 10+64, 104);
	f0.timeToNextFrame = 0.3f;

	Frame f1;
	f1.rect = sf::IntRect(140,40, 140+64, 104);
	f1.timeToNextFrame = 0.3f;

	Frame f2;
	f2.rect = sf::IntRect(270,40, 270+64, 104);
	f2.timeToNextFrame = 0.4f;

	Frame f3;
	f3.rect = sf::IntRect(390,40, 390+64, 104);
	f3.timeToNextFrame = 0.3f;

	Frame f4;
	f4.rect = sf::IntRect(525,40, 525+64, 104);
	f4.timeToNextFrame = 0.3f;

	this->frames.push_back(f0);
	this->frames.push_back(f1);
	this->frames.push_back(f2);
	this->frames.push_back(f3);
	this->frames.push_back(f4);
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

