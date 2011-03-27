#include "ExitPoint.h"
#include <iostream>

ExitPoint::ExitPoint(int w, int h) {
	setSize(w, h);
}

void ExitPoint::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		printf("You are victorious! (But we don't have a victory UI.)\n");
	}
}

void ExitPoint::setMap(std::string filename) {
	mapname = filename;
	std::cout << "setting map exit to " << filename << "\n";
}

std::string ExitPoint::getMap() {
	return mapname;
}