#include "Teleport.h"
#include "Player.h"

TeleportEnter::TeleportEnter(int w, int h, std::string teleName) {
	setSize(w, h);
	name = teleName;
}

void TeleportEnter::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		for (list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		{
			if ((*it)->isTeleportExit() && ((TeleportExit*)*it)->getName() == name)
			{
				otherActor.setPos((*it)->pos_x, (*it)->pos_y);
				// Make the player fly up a bit
				((Player&)otherActor).speed_x = 0;
				((Player&)otherActor).speed_y = -500;
				break;
			}
		}
	}
}

TeleportExit::TeleportExit(std::string teleName) {
	name = teleName;
}
