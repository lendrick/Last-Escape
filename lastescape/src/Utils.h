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

#ifndef UTILS_H
#define UTILS_H

#include <QtScript>
#include <string>
#include <chipmunk/chipmunk.h>
#include <SFML/Graphics.hpp>

using namespace std;

struct Point {
  int x;
  int y;
  Point() {
    x=0;
    y=0;
  }
};

string trim(string s, char c);
string parse_section_title(string s);
double clamp(double n, double min, double max);

void parse_key_pair(string s, string &key, string &val);

cpShape * boxWithOffset(cpBody * body, double width, double height, sf::Vector2f offset);
cpShape * boxWithOffset(cpBody * body, double width, double height, cpVect offset);

QScriptValue runScript(QString filename) ;

#endif
