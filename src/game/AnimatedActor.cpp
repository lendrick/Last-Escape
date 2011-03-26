 #include "AnimatedActor.h"

AnimatedActor::AnimatedActor(sf::Image& image)
{
	//testing values
	this->setOrigin(64.0f, 400.0f);
	this->setSize(24, 48);
	this->sprite.SetImage(image);
//	this->sprite.Resize(24, 48);

	// Set walk-animation TODO read animations from config file
	Animation* walkAnimation = new Animation(this->sprite);
	walkAnimation->toDefaultXeonWalkAnimation();
	this->animations["walk"] = walkAnimation;
	
	Animation* jumpAnimation = new Animation(this->sprite);
	jumpAnimation->toDefaultXeonJumpAnimation();
	this->animations["jump"] = jumpAnimation;

	this->setCurrentAnimation("jump");

//	this->animationQueue.push(new Animation(&this->sprite));
//	this->animationQueue.front()->toDefaultAnimation();
}

AnimatedActor::~AnimatedActor()
{
}

void AnimatedActor::update(float dt)
{
	Actor::update(dt);
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

void AnimatedActor::setCurrentAnimation(std::string name)
{
	if(this->currentAnimation != this->animations[name])
	this->currentAnimation = this->animations[name];
}
