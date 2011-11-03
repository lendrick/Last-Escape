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
extern sf::Uint32 time_step;

namespace Facing {
	enum Enum {
		None,
		Up,
		Down,
		Left,
		Right
	};
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
		Enemy,
		Ground,
		Sensor,
		Wall,
		Death,
		Item,
		Neutral,
		PlayerBullet,
		CentipedeBullet,
		Bumper,
		Crate,
		Door
	};
};

namespace PhysicsGroup {
	enum Enum {
		PlayerBullets = 1,
		EnemyBullets = 2,
		Bumpers = 3
	};
};

namespace Collision {
	enum Enum {
		None = 0,
		Tile = 1,
		Danger = 2,
		SlantBottomRight = 3,
		SlantBottomLeft = 4,
		SlantTopLeft = 5,
		SlantTopRight = 6
	};
};

namespace Gamestatus {
	enum Enum {
		ingame = 1,
		mainMenu = 2,
		gameOver = 3,
		win = 4
	};
}

extern bool paused;

extern ImageCache imageCache;
extern SoundCache soundCache;

double frand(double lower, double upper);

double deg2rad(double deg);
double rad2deg(double rad);
extern Gamestatus::Enum gamestatus;

extern sf::View uiView;
extern sf::View gameView;
