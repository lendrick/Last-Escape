#include "AnimatedActor.h"
#include "ImageCache.h"
#include "globals.h"

AnimatedActor::AnimatedActor(std::string filename)
:Actor()
{
	init();
	setImage(filename);
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
	currentAnimation = NULL;
	dying = false;
        immunityTime = 0.2f;
        damageTimer = 0.0f;
}

void AnimatedActor::setImage(std::string filename) 
{
	hasImage = true;
	setFrameSize(0, 0);
	this->sprite.SetImage(*(imageCache[filename]));
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
			onAnimationComplete(animationName());
		}
		else
		{
			this->currentAnimation->update();
		}
	}
	
	float redness = 0;
        int alpha = 255; 
	if(immunityTime > 0) {
                redness = damageTimer / immunityTime;
                alpha = 128;
        }
        
        int colorLevel = (int) (127.0f + 128.0f * (1.0f - redness));
        
        if(damageTimer > 0)
                sprite.SetColor(sf::Color(255, colorLevel, colorLevel, alpha));
        else
                sprite.SetColor(sf::Color(255, 255, 255, 255));
        
	Actor::draw();
}

void AnimatedActor::doUpdate(float dt) {
        if(damageTimer > 0.0f)
          damageTimer -= dt;
        else
          damageTimer = 0.0f;
        
        update(dt);
}

std::string AnimatedActor::animationName() {
	if(currentAnimation == NULL) {
		return "";
	} else {
		return currentAnimation->getName();
	}
}

void AnimatedActor::setCurrentAnimation(std::string name, bool reset)
{
	if(this->currentAnimation != this->animations[name])
	{
		this->currentAnimation = this->animations[name];
		this->currentAnimation->updateFrame();
		if(reset)
			this->currentAnimation->reset();
		
		//cout << name << endl;
	}
}

Animation* AnimatedActor::getCurrentAnimation() {
	return currentAnimation;
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

void AnimatedActor::setFacing(int direction) {
	facing_direction = direction;
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
							newAnimation->addFrame(number, timeToNextFrame);
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

void AnimatedActor::doDamage(float damage) {
        if(damageTimer <= 0) {
                life -= damage;
                if(life <= 0) die();
                damageTimer = immunityTime;
                onDamage();
        }
}

void AnimatedActor::onDamage() {
        //TODO: play a sound
}