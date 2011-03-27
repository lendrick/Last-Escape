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