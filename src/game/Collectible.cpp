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
		((Player&)otherActor).energy += 5.f;
	}
}


CollectibleWeaponUpgrade::CollectibleWeaponUpgrade() {
	setPlaceholder(sf::Color(0, 255, 255), 16, 16);
}

void CollectibleWeaponUpgrade::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		destroy();
		((Player&)otherActor).current_weapon += 1;
	}
}


CollectibleArmor::CollectibleArmor() {
	setPlaceholder(sf::Color(0, 255, 255), 16, 16);
}

void CollectibleArmor::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		destroy();
		((Player&)otherActor).armor += 1;
	}
}


