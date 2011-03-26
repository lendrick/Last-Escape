#pragma once

#include "SFML/Graphics.hpp"
#include <list>
#include <cstdio>
#include <iostream>

#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

using namespace std;

class Actor;
class AnimatedActor;
class Map;
class Player;
class Input;
class Sound;

extern list<Actor *> actors;
extern Map *game_map;
extern sf::RenderWindow *App;
extern Player *g_player;
extern Input input;

enum Facing {
	FACING_NONE,
	FACING_UP,
	FACING_DOWN,
	FACING_LEFT,
	FACING_RIGHT
};

extern Sound * backgroundMusic;
extern Sound * fireSound;
extern Sound * damageSound;
extern Sound * deathSound;
extern Sound * bulletHitSound;
extern Sound * enemyDeathSound;