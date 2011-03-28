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

/*
 *  Sound.h
 *  Energy
 *
 *  Created by Chris Hardee on 3/25/11.
 *
 */

#include "Sound.h"

//for testing
//#include <iostream>

Sound::Sound()
{
}

Sound::Sound(string filename) {
	loadSound(filename);
}

Sound::~Sound() {
}

bool Sound::loadSound(string filename)
{
	file = filename;
	return song.OpenFromFile(("audio/" + filename).c_str());
}

void Sound::playSound()
{
	//std::cout << "play sound " << file << "\n";
	song.Play();
}

void Sound::setLoop(bool loop)
{
	song.SetLoop(loop);
}

void Sound::stop()
{
	song.Stop();
}

sf::Sound::Status Sound::getStatus()
{
	return song.GetStatus();
}
