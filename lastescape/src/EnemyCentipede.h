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
#include "EnemyPatroller.h"
#include <SFML/Graphics.hpp>

class EnemyCentipede : public EnemyPatroller
{
public:
  EnemyCentipede(double x, double y);
  virtual void updateCallback(double dt);
  virtual void animationCompleteCallback(std::string anim);

private:
  double shootInterval;
  double lastShot;
  double time;
};

class EnemyCentipedeProjectile : public Enemy
{
public:
  EnemyCentipedeProjectile(int direction, int start_x, int start_y);
  virtual void updateCallback(double dt);
  virtual void animationCompleteCallback(std::string anim);
	virtual void collideCallback(Actor &otherActor);
private:

  double fly_speed;
};
