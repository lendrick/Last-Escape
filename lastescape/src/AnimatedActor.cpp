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

#include "AnimatedActor.h"
#include "ImageCache.h"
#include "SoundCache.h"
#include "globals.h"
#include "Map.h"
#include "Player.h"

AnimatedActor::AnimatedActor(double x, double y, double w, double h, std::string filename,  bool staticBody)
  :Actor(x, y, w, h, staticBody)
{
  init();
	actorName = "Animated Actor";
  setImage(filename);
}

AnimatedActor::AnimatedActor(double x, double y, double w, double h, bool staticBody)
  :Actor(x, y, w, h, staticBody)
{
  init();
	actorName = "Animated Actor";
}

AnimatedActor::AnimatedActor(double x, double y, std::string filename)
	:Actor(x, y, 0, 0, false)
{
	//cout << "new actor from file: " << filename << "\n";
	actorName = "Animated Actor";
	loadAnimationsFromFile(filename);
	init();
}

AnimatedActor::~AnimatedActor()
{
}

void AnimatedActor::init()
{
  facing_direction = Facing::Right;
  currentAnimation = NULL;
  dying = false;
  immunityTime = 0.05f;
	damageTimer = 0.1f;
}

void AnimatedActor::setImage(std::string filename)
{
  hasImage = true;
	setFrameSize(0, 0);
  this->sprite.SetImage(*(imageCache[filename]));
  this->currentAnimation = NULL;
}

void AnimatedActor::updateSpriteFacing()
{
  if (facing_direction == Facing::Right) {
    sprite.FlipX(false);
  } else if (facing_direction == Facing::Left) {
    sprite.FlipX(true);
  }
}

Animation * AnimatedActor::addAnimation(std::string name)
{
  if(frame_w == 0 || frame_h == 0) {
    if (debugMode)
      cout << "ERROR: Must set frame size before adding animation.\n";
  }
  Animation* a = new Animation(this->sprite, name);
  this->animations[name] = a;
  a->setFrameSize(frame_w, frame_h);
  return a;
}

void AnimatedActor::setFrameSize(int fw, int fh)
{
  frame_w = fw;
  frame_h = fh;
}

void AnimatedActor::draw()
{
  if(currentAnimation != NULL) {
    if(currentAnimation->getIsFinished()) {
      //TODO jump to next Animation in queue or idle Animation
      this->currentAnimation->setIsFinished(false);
      animationCompleteCallback(animationName());
    } else {
      this->currentAnimation->update();
    }
  }

  double redness = 0;
  if(immunityTime > 0) {
    redness = damageTimer / immunityTime;
  }

  int colorLevel = (int) (255.0f * (1.0f - redness));

  if(damageTimer > 0)
    sprite.SetColor(sf::Color(255, colorLevel, colorLevel, 128));
  else
    sprite.SetColor(sf::Color(255, 255, 255, 255));

  Actor::draw();
}

void AnimatedActor::doUpdate(double dt)
{
  if(damageTimer > 0.0f)
    damageTimer -= dt;
  else
    damageTimer = 0.0f;

  Actor::doUpdate(dt);
}

std::string AnimatedActor::animationName()
{
  if(currentAnimation == NULL) {
    return "";
  } else {
    return currentAnimation->getName();
  }
}

void AnimatedActor::setCurrentAnimation(std::string name, bool reset)
{
  if(this->currentAnimation != this->animations[name]) {
    this->currentAnimation = this->animations[name];
    this->currentAnimation->updateFrame();
    if(reset)
      this->currentAnimation->reset();

    //cout << name << endl;
  }
}

Animation* AnimatedActor::getCurrentAnimation()
{
  return currentAnimation;
}


void AnimatedActor::resetCurrentAnimation()
{
  this->currentAnimation->reset();
}

void AnimatedActor::flipDirection()
{
  if(facing_direction == Facing::Left) {
    facing_direction = Facing::Right;
  } else if(facing_direction == Facing::Right) {
    facing_direction = Facing::Left;
  } else if(facing_direction == Facing::Up) {
    facing_direction = Facing::Down;
  } else if(facing_direction == Facing::Down) {
    facing_direction = Facing::Up;
  }
}

void AnimatedActor::setFacing(int direction)
{
  facing_direction = direction;
}


void AnimatedActor::loadAnimationsFromFile(std::string filename)
{
	//cout << "loadAnimationsFromFile: " << filename << "\n";
	/*
  if(this->sprite.GetImage() == NULL) {
    if (debugMode) {
      cout << "Error: tried to set Animations before an image was set" << endl;
      cout << "animations/" << filename << " was ignored" << endl;
    }
    return;
  }
	*/

  TiXmlDocument doc;
  if (!doc.LoadFile(("animations/" + filename).c_str())) {
		cout << "failed to open animation " << filename << "\n";
    return;
  }

  TiXmlElement* root = doc.RootElement();
	int frameWidth, frameHeight, width, height, offsetX, offsetY;
	std::string image;

  root->QueryIntAttribute("frameWidth",  &frameWidth);
  root->QueryIntAttribute("frameHeight", &frameHeight);
	root->QueryIntAttribute("width",  &width);
	root->QueryIntAttribute("height", &height);
	root->QueryIntAttribute("drawOffsetX",  &offsetX);
	root->QueryIntAttribute("drawOffsetY", &offsetY);
	image = root->Attribute("image");

	/*
	cout << "Setting image for " << actorName << ": " << image << "\n";
	cout << "Frame size " << frameWidth << " " << frameHeight << "\n";
	cout << "Draw offset " << offsetX << " " << offsetY << "\n";
	cout << "Size " << width << " " << height << "\n";
	*/

	this->setImage(image);
	this->setFrameSize(frameWidth, frameHeight);
	this->setDrawOffset(offsetX, offsetY);
	this->setSize(width, height);


  for (TiXmlNode* child = root->FirstChild(); child; child = child->NextSibling()) {
    std::string childName = child->Value();
    //Create an Animation
    if (childName == "animation") {
      //Add to animations map with aName
      std::string aName = ((TiXmlElement*)child)->Attribute("name");
      Animation *newAnimation = this->addAnimation(aName);

      //Set doLoop
      const char* aDoLoopTmp = ((TiXmlElement*)child)->Attribute("doLoop");
      if(aDoLoopTmp != NULL) {
        std::string aDoLoop = aDoLoopTmp;
        if(aDoLoop.size() > 0) {
          char valueStartsWith = aDoLoop.at(0);
          if(valueStartsWith == 't' || valueStartsWith == 'T' || valueStartsWith == '1') {
            newAnimation->setDoLoop(true);
          }
        }
      }

      for (TiXmlNode* aChild = child->FirstChild(); aChild; aChild = aChild->NextSibling()) {
        std::string aChildName = aChild->Value();
        if(aChildName == "frames") {
          for (TiXmlNode* fChild = aChild->FirstChild(); fChild; fChild = fChild->NextSibling()) {
            std::string fChildName = fChild->Value();
            if(fChildName == "frame") {
              int number = 0;
              float timeToNextFrame = 0.f;
              ((TiXmlElement*)fChild)->QueryIntAttribute("number", &number);
              ((TiXmlElement*)fChild)->QueryFloatAttribute("time", &timeToNextFrame);
              newAnimation->addFrame(number, timeToNextFrame);
            } else {
              if (debugMode) {
                cout << "Unkown Tag:" << aChildName << endl;
                cout << "Expected 'frame' tag" << endl;
              }
            }
          }
        } else {
          if (debugMode) {
            cout << "Unkown Tag:" << aChildName << endl;
            cout << "Expected 'frames' tag" << endl;
          }
        }
      }
    } else {
      if (debugMode) {
        cout << "Unkown Tag:" << childName << endl;
        cout << "Expected 'animation' tag" << endl;
      }
    }
  }
}

bool AnimatedActor::doDamage(double damage, bool knockback)
{
  (void)knockback;
  if(damageTimer <= 0) {
    life -= damage;
    if(life <= 0) {
      die();
      return true;
    } else {
      damageCallback();
    }
    damageTimer = immunityTime;
  }
  return false;
}

void AnimatedActor::damageCallback()
{
  soundCache["hit1.ogg"]->play();
}
