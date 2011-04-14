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
/*
 *  Sound.h
 *  Energy
 *
 *  Created by Chris Hardee on 3/25/11.
 *
 */


#include <SFML/Audio.hpp>
#include <string>
#include <QtCore>
using namespace std;

class Sound : public QObject
{
	Q_OBJECT
private:
  sf::Music song;
  string file;

public:
  Sound();
  Sound(string filename);
  ~Sound();

  bool loadSound(string filename);

public slots:
  void play();
  void setLoop(bool);
  void stop();
	sf::Sound::Status getStatus();

};

