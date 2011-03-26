#include "Animation.h"
#include <SFML/Graphics.hpp>

Animation::Animation(sf::Sprite& sprite, std::string name)
: sprite(sprite)
{
	this->name = name;
	this->animationTimer.Reset();
	this->frameIterator = 0;
	this->isFinished = false;
	this->doLoop = false;
}

Animation::~Animation()
{
}

void Animation::setFrameSize(int fw, int fh) {
	frame_w = fw;
	frame_h = fh;
}

void Animation::addFrame(int num, float duration) {
	const sf::Image * img = sprite.GetImage();
	int w = img->GetWidth();
	int h = img->GetHeight();
	
	//std::cout << w << "x" << h << "\n";
	
	int x_tiles = w / frame_w;
	int y_tiles = h / frame_h;
	//std::cout << frame_w << "x" << frame_h << "\n";
	//std::cout << x_tiles << "x" << y_tiles << "\n";
	//std::cout << num << "\n";
	
	int column = num % y_tiles;
	int row = (num - column) / x_tiles;
	
	Frame frame;
	frame.rect = sf::IntRect(column * frame_w, row * frame_h, column * frame_w + frame_w, row * frame_h + frame_h);
	frame.timeToNextFrame = duration;
	frame.number = num;
	frames.push_back(frame);	
}

void Animation::setLoop(bool loop) {
	doLoop = loop;
}

void Animation::reset() {
	frameIterator = 0;
	updateFrame();
	this->animationTimer.Reset();
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
	//cout << "frame update " << frameIterator << "\n";
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
				//cout << " > " << frameIterator << "\n";
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
			//cout << " > " << frameIterator << "\n";
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
	cout << name << " " << frameIterator << ":" << frames.at(frameIterator).number << 
		" (" << frames.at(frameIterator).timeToNextFrame << ")\n";
	
}
