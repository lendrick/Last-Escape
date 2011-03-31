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

#ifndef ENEMY_H
#define ENEMY_H

#include "AnimatedActor.h"
#include "Sound.h"

class Enemy : public AnimatedActor
{
public:
	Enemy(float x, float y, float w, float h);
	
	virtual void collide(Actor& otherActor);

	virtual bool isEnemy() { return true; }

	Sound* fireSound;
	
	void patrol(float dt);
protected:
	float speed_x, speed_y;
};



#endif
