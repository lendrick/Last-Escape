#include "Collectible.h"

#include "Player.h"

Collectible::Collectible()
:AnimatedActor()
{
}


void Collectible::init() 
{
	width = 32;
	height = 32;
	xOrigin = width/2;
	yOrigin = height/2;
	setDrawOffset(16, 16);
	setFrameSize(32, 32);
	
	Animation * tmp;
	tmp = addAnimation("image");
	tmp->addFrame(0, .1f);
	setCurrentAnimation("image");
}

CollectiblePill::CollectiblePill()
:Collectible()
{
	this->setImage("smallenergy.png");
	init();
}

void CollectiblePill::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		destroy();
		((Player&)otherActor).energy += 5.f;
	}
}


CollectibleWeaponUpgrade::CollectibleWeaponUpgrade()
:Collectible()
{
	this->setImage("plasmaball.png");
	init();
}

void CollectibleWeaponUpgrade::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		destroy();
		((Player&)otherActor).current_weapon += 1;
	}
}


CollectibleArmor::CollectibleArmor()
:Collectible() {
	this->setImage("shield.png");
	init();
}

void CollectibleArmor::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		destroy();
		((Player&)otherActor).armor += 1;
	}
}


