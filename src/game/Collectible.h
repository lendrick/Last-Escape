#include "Actor.h"

class Collectible : public Actor
{
public:
	Collectible();
};

class CollectiblePill : public Collectible
{
public:
	CollectiblePill();
	virtual void collidePlayer(Player& player);
};

