#include "ImageCache.h"

#include <list>
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
		sf::Image * img = (*i).second;
		images.erase(i);
		delete img;
	}
}

sf::Image * ImageCache::getImage(std::string name)
{
	if(images.find(name) == images.end()) {
		sf::Image * img = loadImage(name);
		images[name] = img;
	} 
	
	return images[name];
}

void ImageCache::unload(std::string name)
{
	sf::Image * img = images[name];
	images.erase(name);
	delete img;
}

sf::Image * ImageCache::loadImage(std::string filename) 
{
	sf::Image * img = new sf::Image;
	if(!img->LoadFromFile("images/" + filename)) {
		std::cout << "Failed to load image " << filename << "\n";
		return NULL;
	}
	
	return img;
}