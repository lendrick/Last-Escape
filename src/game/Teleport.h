#pragma once

#include "Actor.h"

class TeleportEnter : public Actor {
public:
	TeleportEnter(int w, int h, std::string name);

	virtual bool isTeleportEnter() { return true; }

	void collide(Actor& otherActor);
private:
	std::string name;
};

class TeleportExit : public Actor {
public:
	TeleportExit(std::string name);

	virtual bool isTeleportExit() { return true; }

	std::string getName() { return name; }

private:
	std::string name;
};

