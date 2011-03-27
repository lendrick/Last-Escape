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
		fireSound = soundCache["energyblip.ogg"];
		fireSound->playSound();
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
		((Player&)otherActor).upgradeWeapon();
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


CollectibleEnergyBall::CollectibleEnergyBall()
: Collectible() {
	this->setImage("energyball.png");
	width = 32;
	height = 32;
	xOrigin = width/2;
	yOrigin = height/2;
	setDrawOffset(16, 16);
	setFrameSize(32, 32);
	
	Animation * tmp;
	tmp = addAnimation("anim");
	tmp->addFrame(0, .2f);
	tmp->addFrame(1, .2f);
	tmp->addFrame(2, .2f);
	tmp->addFrame(3, .2f);
	tmp->setDoLoop(true);
	setCurrentAnimation("anim");
}

void CollectibleEnergyBall::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		destroy();
		g_player->energyBalls++;
	}
}
