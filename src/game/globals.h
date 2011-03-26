#pragma once

#include <list>

#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

using namespace std;

class Actor;
class Map;

extern list<Actor *> actors;
extern Map * map;

