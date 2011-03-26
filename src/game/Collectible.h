#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "Actor.h"

class Collectible : public Actor
{
public:
	Collectible();

	virtual bool isCollectible() { return true; }
};

class CollectiblePill : public Collectible
{
public:
	CollectiblePill();
	virtual void collidePlayer(Player& player);
};

#endif
