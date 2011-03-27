#pragma once

// Use this actor to signify where on the map the player starts.

class StartPoint : public Actor {
public:
	virtual bool isStartPoint() { return true; }
};
