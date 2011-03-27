#include "ExitPoint.h"

ExitPoint::ExitPoint(int w, int h) {
	setSize(w, h);
}

void ExitPoint::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		printf("You are victorious! (But we don't have a victory UI.)\n");
	}
}

