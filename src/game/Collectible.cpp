#include "Collectible.h"

#include "Player.h"

Collectible::Collectible() {
}


CollectiblePill::CollectiblePill() {
	setPlaceholder(sf::Color(0, 255, 255), 8, 8);
}

void CollectiblePill::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		destroy();
		((Player&)otherActor).energy += 1.f;
	}
}
