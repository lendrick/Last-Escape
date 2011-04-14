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

#include "EnemyWalker.h"
#include "Player.h"
#include "Map.h"
#include "globals.h"
#include "Collectible.h"
#include "Sound.h"
#include "SoundCache.h"

EnemyWalker::EnemyWalker(double x, double y)
  :EnemyPatroller(x, y, 28.0f, 19.0f)
{
  setImage("walker.png");
  walk_speed = 120.f;
  shape->u = 0.1f;

  dying = false;

  setDrawOffset(16, 11);
  setFrameSize(32, 32);

  Animation * tmp;

  //pick a random death sound
  int sound_num = rand() % 19;
  sound_num += 1;
  std::string s;
  std::stringstream out;
  out << sound_num;
  s = out.str();

  std::string sound_file = s + "-BugSplat.ogg";
  //cout << sound_file;
  fireSound = soundCache[sound_file];

  tmp = addAnimation("walk");
  tmp->addFrame(3, .2f);
  tmp->addFrame(2, .2f);
  tmp->addFrame(1, .2f);
  tmp->addFrame(2, .2f);
  tmp->setDoLoop(true);

  tmp = addAnimation("die");
  tmp->addFrame(7, .07f);
  tmp->addFrame(6, .07f);
  tmp->addFrame(5, .07f);
  tmp->addFrame(4, .07f);

  setCurrentAnimation("walk");
}

EnemyWalker::~EnemyWalker()
{
}
