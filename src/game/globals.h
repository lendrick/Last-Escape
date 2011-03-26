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

extern list<Actor *> actors;
extern Map *game_map;
extern sf::RenderWindow *App;
extern Player *g_player;
