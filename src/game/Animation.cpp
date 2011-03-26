
#include "Animation.h"

Animation::Animation(sf::Sprite& sprite)
: sprite(sprite)
{
	this->animationTimer.Reset();
	this->frameIterator = 0;
	this->isFinished = false;
}

Animation::~Animation()
{
}

// testing purposes
void Animation::toDefaultXeonWalkAnimation()
{
//	Frame f0;
//	f0.rect = sf::IntRect(10,40, 10+64, 104);
//	f0.timeToNextFrame = 0.2f;

	Frame f1;
	f1.rect = sf::IntRect(140,40, 140+64, 107);
	f1.timeToNextFrame = 0.2f;

	Frame f2;
	f2.rect = sf::IntRect(270,40, 270+64, 107);
	f2.timeToNextFrame = 0.2f;

	Frame f3;
	f3.rect = sf::IntRect(390,40, 390+64, 107);
	f3.timeToNextFrame = 0.2f;

	Frame f4;
	f4.rect = sf::IntRect(525,40, 525+64, 107);
	f4.timeToNextFrame = 0.2f;

//	this->frames.push_back(f0);
	this->frames.push_back(f1);
	this->frames.push_back(f2);
	this->frames.push_back(f3);
	this->frames.push_back(f4);


	this->doLoop = true;
}

// testing purposes TODO read from config file
void Animation::toDefaultXeonJumpAnimation()
{
	Frame f0;
	f0.rect = sf::IntRect(10,175, 10+64, 175+64);
	f0.timeToNextFrame = 0.3f;

	Frame f4;
	f4.rect = sf::IntRect(525,40, 525+64, 104);
	f4.timeToNextFrame = 0.2f;

	this->frames.push_back(f0);
//	this->frames.push_back(f1);
//	this->frames.push_back(f2);
//	this->frames.push_back(f3);
//	this->frames.push_back(f4);
//	this->frames.push_back(f4);
	this->doLoop = false;
}

void Animation::toDefaultXeonIdleAnimation()
{
	Frame f0;
	f0.rect = sf::IntRect(10,40, 10+64, 104);
	f0.timeToNextFrame = 0.6f;

	Frame f1;
	f1.rect = sf::IntRect(10,38, 10+64, 104);
	f1.timeToNextFrame = 0.4f;

	this->frames.push_back(f0);
	this->frames.push_back(f1);
	this->doLoop = true;
}

bool Animation::getIsFinished()
{
	return this->isFinished;
}
void Animation::setIsFinished(bool value)
{
	this->isFinished = value;
}

void Animation::update()
{
	if(!this->frames.empty() && this->animationTimer.GetElapsedTime() > this->frames.at(frameIterator).timeToNextFrame)
	{
		// is the last Frame in frames-vector.
		if(this->frames.size()-1 == frameIterator)
		{
			//Loop, restart Animation?
			if(this->doLoop)
			{
				frameIterator = 0;
				this->updateFrame();
			}
			else
			{
				this->isFinished = true;
			}
		}
		else
		{
			//Update to the next frame
			this->frameIterator++;
			this->updateFrame();
		}
		this->animationTimer.Reset();
	}
}

void Animation::updateFrame()
{
	if(!this->frames.empty())
		this->sprite.SetSubRect(frames.at(frameIterator).rect );
	else
		cout << "no frames!" << endl;
}
