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

#include "SoundCache.h"
 
#include <map>
#include <iostream>
#include "Sound.h"

SoundCache::SoundCache()
{
}

SoundCache::~SoundCache()
{
	soundPtrMap::iterator i;
	while(!sounds.empty()) {
		i = sounds.begin();
		Sound * snd = (*i).second;
		sounds.erase(i);
		delete snd;
	}
}

Sound * SoundCache::operator[](std::string name) 
{
	return get(name);
}	

Sound * SoundCache::get(std::string name)
{
	if(sounds.find(name) == sounds.end()) {
		Sound * snd = load(name);
		sounds[name] = snd;
	} 
	
	return sounds[name];
}

void SoundCache::unload(std::string name)
{
	Sound * snd = sounds[name];
	sounds.erase(name);
	delete snd;
}

Sound * SoundCache::load(std::string filename) 
{
	Sound * snd = new Sound;
	if(!snd->loadSound(filename)) {
		std::cout << "Failed to load sound " << filename << "\n";
		return NULL;
	}
	
	return snd;
}