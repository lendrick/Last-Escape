#pragma once

#include "SFML/Graphics.hpp"
#include <list>


#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

using namespace std;

class Actor;
class Map;

extern list<Actor *> actors;
extern Map *game_map;
extern sf::RenderWindow *App;


