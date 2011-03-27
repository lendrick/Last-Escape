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
using namespace std;

class Sound
{
private:
	sf::Music song;
	string file;
	
public:
	Sound();
	Sound(string filename);
	~Sound();

	bool loadSound(string filename);
	void playSound();
	void setLoop(bool);

};

