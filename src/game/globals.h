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

enum Facing {
	FACING_NONE,
	FACING_UP,
	FACING_DOWN,
	FACING_LEFT,
	FACING_RIGHT
};

extern bool paused;

extern ImageCache imageCache;
extern SoundCache soundCache;
