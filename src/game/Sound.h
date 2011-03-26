/*
 *  Sound.h
 *  Energy
 *
 *  Created by Chris Hardee on 3/25/11.
 *
 */

#ifndef MAP_H
#define MAP_H

#include <SFML/Audio.hpp>
#include <string>

using namespace std;

class Sound
{
	private:
	sf::Music song;

	public:
	Sound();
	~Sound();

	bool loadSound(string filename);
	void playSound();

};

#endif
