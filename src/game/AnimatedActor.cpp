 #include "AnimatedActor.h"

AnimatedActor::AnimatedActor(sf::Image& image)
:Actor()
{
	init();
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
	init();
}

AnimatedActor::~AnimatedActor()
{
}

void AnimatedActor::init() {
	facing_direction = FACING_RIGHT;
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
