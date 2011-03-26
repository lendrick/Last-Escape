 #include "AnimatedActor.h"

AnimatedActor::AnimatedActor(sf::Image& image)
:Actor()
{
	//testing values
//	this->setOrigin(64.0f, 400.0f);
//	this->setSize(24, 48);
  setImage(image);

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
	this->currentAnimation->update(dt);
}

// implemented for testing
void AnimatedActor::collide(Actor* other)
{
}
