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
