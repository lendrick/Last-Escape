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
#include "Enemy.h"
#include "Bumper.h"

class EnemyPatroller : public Enemy
{
public:
  EnemyPatroller(double x, double y, double w, double h);
  virtual ~EnemyPatroller();
  virtual void updateCallback(double dt);
  virtual void draw();
  virtual void die();
  virtual void animationCompleteCallback(std::string anim);
  virtual void bumperCollideCallback(int facing);
protected:
  double walk_speed;
  Bumper* leftBumper;
  Bumper* rightBumper;
  double drop_offset_x, drop_offset_y;
};

