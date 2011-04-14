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

void Animation::addFrame(int num, double duration) {
	const sf::Image * img = sprite.GetImage();
	int w = img->GetWidth();
	int h = img->GetHeight();
	
	
	//std::cout << "*** " << name << "\n";
	//std::cout << "img dimensions: " << w << "x" << h << "\n";

	int x_tiles = w / frame_w;
	int y_tiles = h / frame_h;
	//std::cout << "frame size: " << frame_w << "x" << frame_h << "\n";
	//std::cout << "image tile grid size: " << x_tiles << "x" << y_tiles << "\n";
	//std::cout << "frame numnber: " << num << "\n";
	
	int column = num % x_tiles;
	int row = (num - column) / x_tiles;
	
	//std::cout << "frame location: " << column << ", " << row << "\n";
	
	int x1 = column * frame_w;
	int y1 = row * frame_h;
	int x2 = x1 + frame_w;
	int y2 = y1 + frame_h;
	
	//std::cout << "rect: (" << x1 << ", " << y1 << ") (" << x2 << ", " << y2 << ")\n";
	Frame frame;
	frame.rect = sf::IntRect(x1, y1, x2, y2);
	frame.timeToNextFrame = duration;
	frame.number = num;
	frames.push_back(frame);	
}

void Animation::reset() {
	frameIterator = 0;
	updateFrame();
	this->animationTimer.Reset();
}

int Animation::getFrame() {
	return frames.at(frameIterator).number;
}

std::string Animation::getName() {
	return name;
}

bool Animation::getIsFinished()
{
	return this->isFinished;
}
void Animation::setIsFinished(bool value)
{
	this->isFinished = value;
}
void Animation::setDoLoop(bool value)
{
	this->doLoop = value;
}
void Animation::addFrame(Frame frame)
{
	this->frames.push_back(frame);
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
	
	/*
	cout << name << " " << frameIterator << ":" << frames.at(frameIterator).number << 
		" (" << frames.at(frameIterator).timeToNextFrame << ")\n";
		*/
	
}
