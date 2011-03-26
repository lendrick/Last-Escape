 #include "AnimatedActor.h"

AnimatedActor::AnimatedActor(sf::Image& image)
{
	//testing values
//	this->setOrigin(64.0f, 400.0f);
//	this->setSize(24, 48);
	this->sprite.SetImage(image);
	this->sprite.SetX(64.f);
	this->sprite.SetY(400.f);
//	this->sprite.Resize(24, 48);

	this->currentAnimation = new Animation(this->sprite);
	this->currentAnimation->toDefaultXeonWalkAnimation(); //testing 

//	this->animationQueue.push(new Animation(&this->sprite));
//	this->animationQueue.front()->toDefaultAnimation();
}

AnimatedActor::~AnimatedActor()
{
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
