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
#include "globals.h"

class Input
{
public:
  Input();
  ~Input();
  void poll();
  int direction();
  bool jump();
  bool quit();
  bool shoot();
  bool crouch();
  bool shooting();
  bool jumping();
  bool crouching();
  bool stopJump();

private:
  void initFrame();

  int inputDirection;
  bool inputQuit;
  bool inputShoot;
  bool inputJump;
  bool inputCrouch;
  bool inputStopJump;
};

typedef struct InputItem {
  sf::Key::Code key;
  sf::String text;
} InputItem;

#define INPUT_JUMP 0
#define INPUT_LEFT 1
#define INPUT_RIGHT 2
#define INPUT_CROUCH 3
#define INPUT_SHOOT 4

extern InputItem inputs[5];
