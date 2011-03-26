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
	this->sprite.SetImage(image);
	this->currentAnimation = NULL;
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

void AnimatedActor::setCurrentAnimation(std::string name)
{
	if(this->currentAnimation != this->animations[name])
	{
		this->currentAnimation = this->animations[name];
		this->currentAnimation->updateFrame();
		cout << name << endl;
	}
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
	TiXmlDocument doc;
	if (!doc.LoadFile(("animations/" + filename).c_str()))
	{
		printf("failed to open map\n");
		return;
	}

	TiXmlElement* root = doc.RootElement();

	for (TiXmlNode* child = root->FirstChild(); child; child = child->NextSibling())
	{
		std::string childName = child->Value();
		if (childName == "animation")
		{
			Animation *newAnimation = new Animation(this->sprite);
			for (TiXmlNode* aChild = child->FirstChild(); aChild; aChild = aChild->NextSibling())
			{
				std::string aChildName = aChild->Value();
				if(aChildName == "name")
				{
					std::string aName = ((TiXmlElement*)aChild)->GetText();
					this->animations[aName] = newAnimation;
					cout << "added Animation with name" << aName << endl;
				}
				else if(aChildName == "doLoop")
				{
					stringstream ss( ((TiXmlElement*)aChild)->GetText() );
					bool aDoLoop;
					ss >> aDoLoop;
					newAnimation->setDoLoop(aDoLoop);
				}
				else if(aChildName == "frames")
				{
					for (TiXmlNode* fChild = aChild->FirstChild(); fChild; fChild = fChild->NextSibling())
					{
						std::string fChildName = child->Value();
						if(fChildName == "frame")
						{
							Frame frame;
							int x, y, xw, yh;
							((TiXmlElement*)fChild)->QueryIntAttribute("x", &x);
							((TiXmlElement*)fChild)->QueryIntAttribute("y", &y);
							((TiXmlElement*)fChild)->QueryIntAttribute("xw", &xw);
							((TiXmlElement*)fChild)->QueryIntAttribute("yh", &yh);
							((TiXmlElement*)fChild)->QueryFloatAttribute("timeToNextFrame", &frame.timeToNextFrame);
							newAnimation->addFrame(frame);
						}
					}
				}
			}
		}
		else
		{
			cout << "Unkown child name" << childName << endl;
		}
	}
}
