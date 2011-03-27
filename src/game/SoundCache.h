#pragma once

#include <map>
#include "Sound.h"

typedef std::map<std::string, Sound *> soundPtrMap;

class SoundCache {
public:
	SoundCache();
	~SoundCache();
	
	soundPtrMap sounds;
	Sound * get(std::string name);
	void unload(std::string name);
	Sound * operator[](std::string name);
private:
	Sound * load(std::string filename);
}; 
