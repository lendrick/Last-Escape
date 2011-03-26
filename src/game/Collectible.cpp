#include "Collectible.h"

#include "Player.h"

Collectible::Collectible() {
}


CollectiblePill::CollectiblePill() {
	setPlaceholder(sf::Color(0, 255, 255), 8, 8);
}

void CollectiblePill::collidePlayer(Player& player) {
	destroy();
	player.energy += 1.f;
}
