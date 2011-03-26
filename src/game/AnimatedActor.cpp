 #include "AnimatedActor.h"

AnimatedActor::AnimatedActor(sf::Image& image)
:Actor()
{
	setImage(image);

	//testing values
	this->setOrigin(64.0f, 400.0f);
	this->setSize(24, 48);
//	this->animationQueue.push(new Animation(&this->sprite));
//	this->animationQueue.front()->toDefaultAnimation();
}

AnimatedActor::AnimatedActor()
:Actor()
{
}

AnimatedActor::~AnimatedActor()
{
}

void AnimatedActor::setImage(sf::Image & image) 
{
	this->sprite.SetImage(image);
	this->sprite.SetX(64.f);
	this->sprite.SetY(400.f);
	this->currentAnimation = NULL;
	//	this->sprite.Resize(24, 48);
}

void AnimatedActor::update(float dt)
{
	Actor::update(dt);
	if(currentAnimation != NULL)
	{
		if(currentAnimation->getIsFinished())
	 	{
			//TODO jump to next Animation in queue or idle Animation
			this->currentAnimation->setIsFinished(false);
		}
		else
		{
			this->currentAnimation->update(dt);
		}
	}
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
