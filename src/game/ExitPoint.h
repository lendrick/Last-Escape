#pragma once

#include "Actor.h"

// Use this actor to signify an map exit.  When the player collides with it,
// go to the next map.

class ExitPoint : public Actor {
public:
	ExitPoint(int w, int h);
};

