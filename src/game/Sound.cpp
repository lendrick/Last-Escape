/*
 *  Sound.h
 *  Energy
 *
 *  Created by Chris Hardee on 3/25/11.
 *
 */

#include "Sound.h"

Sound::Sound()
{
}

bool Sound::loadSound(string filename)
{
	return song.OpenFromFile(("audio/" + filename).c_str());
}

void Sound::playSound()
{
	song.Play();
}
