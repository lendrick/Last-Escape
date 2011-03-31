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

#include "ExitPoint.h"
#include <iostream>

ExitPoint::ExitPoint(float x, float y, float w, float h)
:Sensor(x, y, w, h)
{
}

void ExitPoint::collide(Actor& otherActor) {
	if (otherActor.isPlayer())
	{
		printf("You are victorious! (But we don't have a victory UI.)\n");
	}
}

void ExitPoint::setMap(std::string filename) {
	mapname = filename;
	if (debugMode)
		std::cout << "setting map exit to " << filename << "\n";
}

std::string ExitPoint::getMap() {
	return mapname;
}
