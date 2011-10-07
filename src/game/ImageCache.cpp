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

#include "ImageCache.h"

#include <map>
#include <SFML/Graphics.hpp>
#include <iostream>

ImageCache::ImageCache()
{
}

ImageCache::~ImageCache()
{
	imagePtrMap::iterator i;
	while(!images.empty()) {
		i = images.begin();
		sf::Texture * img = (*i).second;
		images.erase(i);
		delete img;
	}
}

sf::Texture * ImageCache::operator[](std::string name)
{
	return getImage(name);
}

sf::Texture * ImageCache::getImage(std::string name)
{
	if(images.find(name) == images.end()) {
		sf::Texture * img = loadImage(name);
		images[name] = img;
	}

	return images[name];
}

void ImageCache::unload(std::string name)
{
	sf::Texture * img = images[name];
	images.erase(name);
	delete img;
}

sf::Texture * ImageCache::loadImage(std::string filename)
{
	sf::Texture * img = new sf::Texture;
	if(!img->LoadFromFile("images/" + filename)) {
		std::cout << "Failed to load image " << filename << "\n";
		return NULL;
	} else {
	  img->SetSmooth(false);
	}

	return img;
}
