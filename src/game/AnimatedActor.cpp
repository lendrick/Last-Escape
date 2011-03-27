#include "AnimatedActor.h"

AnimatedActor::AnimatedActor(sf::Image& image)
:Actor()
{
	init();
	setImage(image);
}

AnimatedActor::AnimatedActor()
:Actor()
{
	init();
}

AnimatedActor::~AnimatedActor()
{
}

void AnimatedActor::init() {
	facing_direction = FACING_RIGHT;
	this->currentAnimation = NULL;
}

void AnimatedActor::setImage(sf::Image & image) 
{
	setFrameSize(0, 0);
	this->sprite.SetImage(image);
	this->currentAnimation = NULL;
}

void AnimatedActor::updateSpriteFacing() {
	if (facing_direction == FACING_RIGHT)
	{
		sprite.FlipX(false);
	}
	else if (facing_direction == FACING_LEFT)
	{
		sprite.FlipX(true);
	}
}

Animation * AnimatedActor::addAnimation(std::string name) {
	if(frame_w == 0 || frame_h == 0) {
		cout << "ERROR: Must set frame size before adding animation.\n";
	}
	Animation* a = new Animation(this->sprite, name);
	this->animations[name] = a;
	a->setFrameSize(frame_w, frame_h);
	return a;
}

void AnimatedActor::setFrameSize(int fw, int fh) {
	frame_w = fw;
	frame_h = fh;
}

void AnimatedActor::draw()
{
	if(currentAnimation != NULL)
	{
		if(currentAnimation->getIsFinished())
	 	{
			//TODO jump to next Animation in queue or idle Animation
			this->currentAnimation->setIsFinished(false);
		}
		else
		{
			this->currentAnimation->update();
		}
	}
	Actor::draw();
}

void AnimatedActor::setCurrentAnimation(std::string name, bool reset)
{
	if(this->currentAnimation != this->animations[name])
	{
		this->currentAnimation = this->animations[name];
		this->currentAnimation->updateFrame();
		if(reset)
			this->currentAnimation->reset();
		
		cout << name << endl;
	}
}

void AnimatedActor::resetCurrentAnimation() {
	this->currentAnimation->reset();
}

void AnimatedActor::flipDirection() {
	if(facing_direction == FACING_LEFT) {
		facing_direction = FACING_RIGHT;
	} else if(facing_direction == FACING_RIGHT) {
		facing_direction = FACING_LEFT;
	} else if(facing_direction == FACING_UP) {
		facing_direction = FACING_DOWN;
	} else if(facing_direction == FACING_DOWN) {
		facing_direction = FACING_UP;
	}
}

void AnimatedActor::loadAnimationsFromFile(std::string filename)
{
	if(this->sprite.GetImage() == NULL)
	{
		cout << "Error: tried to set Animations before an image was set" << endl;
		cout << "animations/" << filename << " was ignored" << endl;
		return;
	}

	TiXmlDocument doc;
	if (!doc.LoadFile(("animations/" + filename).c_str()))
	{
		printf("failed to open map\n");
		return;
	}

	TiXmlElement* root = doc.RootElement();
	int frameWidth, frameHeight;
	root->QueryIntAttribute("frameWidth",  &frameWidth);
	root->QueryIntAttribute("frameHeight", &frameHeight);
	this->setFrameSize(frameWidth, frameHeight);

	for (TiXmlNode* child = root->FirstChild(); child; child = child->NextSibling())
	{
		std::string childName = child->Value();
		//Create an Animation
		if (childName == "animation")
		{
			//Add to animations map with aName
			std::string aName = ((TiXmlElement*)child)->Attribute("name");
			Animation *newAnimation = this->addAnimation(aName);
			cout << "added Animation with name " << aName << endl;
	
			//Set doLoop
			const char* aDoLoopTmp = ((TiXmlElement*)child)->Attribute("doLoop");
			if(aDoLoopTmp != NULL)
			{
				std::string aDoLoop = aDoLoopTmp;
				if(aDoLoop.size() > 0)
				{
					char valueStartsWith = aDoLoop.at(0);
					if(valueStartsWith == 't' || valueStartsWith == 'T' || valueStartsWith == '1')
					{
						newAnimation->setDoLoop(true);
					}
				}
			}
		
			for (TiXmlNode* aChild = child->FirstChild(); aChild; aChild = aChild->NextSibling())
			{
				std::string aChildName = aChild->Value();
				if(aChildName == "frames")
				{
					for (TiXmlNode* fChild = aChild->FirstChild(); fChild; fChild = fChild->NextSibling())
					{
						std::string fChildName = fChild->Value();
						if(fChildName == "frame")
						{
							int number = 0;
							float timeToNextFrame = 0.f;
							((TiXmlElement*)fChild)->QueryIntAttribute("number", &number);
							((TiXmlElement*)fChild)->QueryFloatAttribute("time", &timeToNextFrame);
							cout << "parsed" << timeToNextFrame << endl;
							newAnimation->addFrame(number, timeToNextFrame);
							cout << "Added " << number << " frame" << endl;
						} 
						else
						{
							cout << "Unkown Tag:" << aChildName << endl;
							cout << "Expected 'frame' tag" << endl;
						}
					}
				}
				else
				{
					cout << "Unkown Tag:" << aChildName << endl;
					cout << "Expected 'frames' tag" << endl;
				}
			}
		}
		else
		{
			cout << "Unkown Tag:" << childName << endl;
			cout << "Expected 'animation' tag" << endl;
		}
	}
}
