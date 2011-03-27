#pragma once

#include <list>
#include <SFML/Graphics.hpp>

typedef std::map<std::string, sf::Image *> imagePtrMap;

class ImageCache {
public:
	ImageCache();
	~ImageCache();
	
	imagePtrMap images;
	sf::Image * getImage(std::string name);
	void unload(std::string name);
private:
	sf::Image * loadImage(std::string filename);
};