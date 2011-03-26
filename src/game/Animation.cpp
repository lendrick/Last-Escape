#include "Animation.h"
#include <SFML/Graphics.hpp>

Animation::Animation(sf::Sprite& sprite)
: sprite(sprite)
{
	this->animationTimer.Reset();
	this->frameIterator = 0;
	this->isFinished = false;
	this->doLoop = false;
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

/*
	Frame f1;
	f1.rect = sf::IntRect(0, 0, 128, 128);
	f1.timeToNextFrame = 0.2f;

	Frame f2;
	f2.rect = sf::IntRect(128, 0, 256, 128);
	f2.timeToNextFrame = 0.2f;

	Frame f3;
	f3.rect = sf::IntRect(256, 0, 384, 128);
	f3.timeToNextFrame = 0.2f;

	Frame f4;
	f4.rect = sf::IntRect(384, 0, 512, 128);
	f4.timeToNextFrame = 0.2f;

//	this->frames.push_back(f0);
	this->frames.push_back(f1);
	this->frames.push_back(f2);
	this->frames.push_back(f3);
	this->frames.push_back(f4);
	*/

	setFrameSize(128, 128);

	addFrame(0, .2f);
	addFrame(1, .2f);
	addFrame(2, .2f);
	addFrame(3, .2f);

	setLoop(true);
}

// testing purposes TODO read from config file
void Animation::toDefaultXeonJumpAnimation()
{
	/*
	Frame f0;
	f0.rect = sf::IntRect(0, 128, 128, 256);
	f0.timeToNextFrame = 0.3f;

	this->frames.push_back(f0);
	this->doLoop = false;
	*/
	setFrameSize(128, 128);
	
	addFrame(0, .2f);
	
	setLoop(false);
}

void Animation::toDefaultXeonIdleAnimation()
{
	/*
	Frame f0;
	f0.rect = sf::IntRect(0, 0, 128, 128);
	f0.timeToNextFrame = 0.6f;

	Frame f1;
	f1.rect = sf::IntRect(0, -2, 128, 126);
	f1.timeToNextFrame = 0.4f;

	this->frames.push_back(f0);
	this->frames.push_back(f1);
	this->doLoop = true;
	*/
	setFrameSize(128, 128);
	
	addFrame(0, .2f);
	
	setLoop(false);
}

void Animation::setFrameSize(int fw, int fh) {
	frame_w = fw;
	frame_h = fh;
}

void Animation::addFrame(int num, float duration) {
	const sf::Image * img = sprite.GetImage();
	int w = img->GetWidth();
	int h = img->GetHeight();
	
	std::cout << w << "x" << h << "\n";
	
	int x_tiles = w / frame_w;
	int y_tiles = h / frame_h;
	std::cout << frame_w << "x" << frame_h << "\n";
	std::cout << x_tiles << "x" << y_tiles << "\n";
	std::cout << num << "\n";
	
	int column = num % y_tiles;
	int row = (num - column) / x_tiles;
	
	Frame frame;
	frame.rect = sf::IntRect(column * frame_w, row * frame_h, column * frame_w + frame_w, row * frame_h + frame_h);
	frame.timeToNextFrame = duration;
	frames.push_back(frame);	
}

void Animation::setLoop(bool loop) {
	doLoop = loop;
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
