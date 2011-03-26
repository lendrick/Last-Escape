 #include "AnimatedActor.h"

AnimatedActor::AnimatedActor(sf::Image& image)
:Actor()
{
	//testing values
  setImage(image);

	// Set walk-animation TODO read animations from config file
	Animation* walkAnimation = new Animation(this->sprite);
	walkAnimation->toDefaultXeonWalkAnimation();
	this->animations["walk"] = walkAnimation;
	
	Animation* jumpAnimation = new Animation(this->sprite);
	jumpAnimation->toDefaultXeonJumpAnimation();
	this->animations["jump"] = jumpAnimation;

	this->setCurrentAnimation("jump");
>>>>>>> origin/master

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
	//	this->sprite.Resize(24, 48);
	
	this->currentAnimation = new Animation(this->sprite);
	this->currentAnimation->toDefaultXeonWalkAnimation(); //testing 
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
