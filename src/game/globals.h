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

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <cstdio>
#include <iostream>
#include <string>

#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

using namespace std;

class Actor;
class AnimatedActor;
class Map;
class Player;
class Input;
class Sound;
class ImageCache;
class SoundCache;

extern list<Actor *> actors;
extern Map *game_map;
extern sf::RenderWindow *App;
extern Player *g_player;
extern Input input;
extern bool godMode;
extern sf::Music backgroundMusic;
extern bool enableMusic;
extern std::string startMap;
extern bool debugMode;
extern const double time_step;

enum Facing {
	FACING_NONE,
	FACING_UP,
	FACING_DOWN,
	FACING_LEFT,
	FACING_RIGHT
};

namespace PhysicsLayer {
	enum Enum {
		Map = 1,
		Player = 2,
		Enemy = 4,
		PlayerBullet = 8,
		EnemyBullet = 16
	};
};

namespace PhysicsType {
	enum Enum {
		Player = 1,
		Enemy = 2,
		Ground = 3,
		Sensor = 4,
		Wall = 5
	};
};

extern bool paused;

extern ImageCache imageCache;
extern SoundCache soundCache;
