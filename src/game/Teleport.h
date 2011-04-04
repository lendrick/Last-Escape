/*
 *  This file is part of Last Escape.
 *
 *  Last Escape is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Last Escape is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Last Escape.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Actor.h"
#include "Sensor.h"

class TeleportEnter : public Sensor {
public:
	TeleportEnter(double x, double y, double w, double h, std::string name);

	virtual bool isTeleportEnter() { return true; }

	void collide(Actor& otherActor);
private:
	std::string name;
};

class TeleportExit : public Sensor {
public:
	TeleportExit(double x, double y, std::string name);

	virtual bool isTeleportExit() { return true; }

	std::string getName() { return name; }

private:
	std::string name;
};

