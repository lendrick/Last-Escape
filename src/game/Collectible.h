#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "AnimatedActor.h"
#include "Sound.h"
#include "SoundCache.h"

class Collectible : public AnimatedActor
{
public:
	Collectible();
	
	Sound* fireSound;
	SoundCache soundCache;

	virtual bool isCollectible() { return true; }
	
protected:
	void init();
};

class CollectiblePill : public Collectible
{
public:
	CollectiblePill();
	virtual void collide(Actor& otherActor);
};

class CollectibleWeaponUpgrade : public Collectible
{
public:
	CollectibleWeaponUpgrade();
	virtual void collide(Actor& otherActor);
};


class CollectibleArmor : public Collectible
{
public:
	CollectibleArmor();
	virtual void collide(Actor& otherActor);
};

class CollectibleEnergyBall : public Collectible
{
public:
	CollectibleEnergyBall();
	virtual void collide(Actor& otherActor);
};

#endif
