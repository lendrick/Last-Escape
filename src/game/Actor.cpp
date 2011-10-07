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

#include "Actor.h"
#include "Map.h"
#include "globals.h"
#include <list>
#include <cmath>

Actor::Actor(double x, double y, double w, double h, bool staticBody) {
	width = w;
	height = h;
// 	setPos(0, 0);
	destroyed = false;
	actors.push_back(this);
	setDrawOffset(0, 0);
	collideable = true;
	hasImage = false;
	hidden = false;
	currentLevel = 1;
	experienceValue = 0;
	body = NULL;
	shape = NULL;
	grounded = 0;
	actorName = "Unnamed Actor";
	shapeLayers = CP_ALL_LAYERS;
	this->staticBody = staticBody;
	defaultVelocityFunc = cpBodyUpdateVelocity;
	static_x = static_y = 0;
	toTeleport = false;
	teleport_x = teleport_y = 0;
	teleport_vx = teleport_vy = 0;
	awake = true;
	canSleep = true;
	destroyedCount = 0;
	resetPhysics(x, y);
}

Actor::~Actor() {
	//cout << "delete actor " << actorName << "\n";
	destroyPhysics();
}

void Actor::setPlaceholder(sf::Color c, double w, double h, double xoff, double yoff) {
	width = (int)w;
	height = (int)h;
	sprite.SetColor(c);
	sprite.SetScale((double)width, (double)height);
	sprite.SetOrigin(xoff, yoff);
}

void Actor::setVelocityFunc(cpBodyVelocityFunc f) {
	if(body)
		body->velocity_func = f;
	defaultVelocityFunc = f;
}

void Actor::setDrawOffset(int ox, int oy) {
	xDrawOffset = ox;
	yDrawOffset = oy;
	sprite.SetOrigin((double)ox, (double)oy);
}

void Actor::getPos(double &px, double &py) {
	if(body) {
		px = body->p.x;
		py = body->p.y;
	} else if(shape) {
		px = static_x;
		py = static_y;
	} else {
		px = py = 0;
	}
}

void Actor::setSize(int w, int h) {
	height = h;
	width = w;
	//resetPhysics();
}

void Actor::getSize(int &w, int &h) {
	h = height;
	w = width;
}

bool Actor::isOnCamera() {
	double px, py;
	getPos(px, py);
	if(body) py += height;
	sf::FloatRect cam = getRectFromView(gameView);
	
	double radius = height + width;  // manhattan distance, for speed.
	if(px > cam.Left - radius && px < cam.Left + cam.Width + radius && py > cam.Top - cam.Height - radius && py < cam.Top + radius)
		return true;
	else
		return false;
}

void Actor::draw() {
	if(!hidden && hasImage) {
		cpVect pos;
		double px, py;
		getPos(px, py);
		//if(body) py += height;
		sf::FloatRect cam = getRectFromView(gameView);
		
		double radius = height + width;  // manhattan distance, for speed.
		if(px > cam.Left - radius && px < cam.Left + cam.Width + radius && py > cam.Top - cam.Height - radius && py < cam.Top + radius) {
			if(body && body->a != 0) {
				sprite.SetRotation(-rad2deg(body->a));
				//if(body->a > 0)
					//cout << actorName << " rotation " << rad2deg(body->a) <<"\n";
			} else {
				sprite.SetRotation(0);
			}
					
			sprite.FlipY(true);
			sprite.SetPosition(px, py);
			App->Draw(sprite);
		
			if(debugMode)
			{
				double px, py;
				getPos(px, py);
				
				double bbx1, bby1, bbx2, bby2;
				bbx1 = px - width/2;
				bby1 = py - height/2;
				bbx2 = bbx1 + width;
				bby2 = bby1 + height;
				
				sf::Shape rect = sf::Shape::Rectangle(-width/2, -height/2, width/2, height/2,
																				sf::Color(0, 0, 0, 0), 1.0f, sf::Color(0, 255, 255));
				rect.SetPosition(px, py);
				if(body && body->a) {
					rect.SetRotation(-rad2deg(body->a));
				}
				App->Draw(rect);
				
				// Draw a crosshair at the actor's position
				App->Draw(sf::Shape::Line(px-4, py, px+4, py, 1.0f, 
																				sf::Color(0, 0, 255)));
				App->Draw(sf::Shape::Line(px, py-4, px, py+4, 1.0f,
																				sf::Color(0, 0, 255)));
      }
		}
	}
}

void Actor::destroy() {
	onDestroy();
	game_map->actorDestroyed(this);
	destroyed = true;
}

bool Actor::isGrounded() {
	//return game_map->isGrounded(*this);
	return(grounded > 0);
}

bool Actor::isDestroyed() {
	return destroyed;
}

bool Actor::isDying() {
	return dying;
}

void Actor::die() {
	destroy();
}

bool Actor::canCollide() {
	return collideable;
}

void Actor::setCanCollide(bool col) {
	collideable = col;
	if(col) {
		shape->layers = shapeLayers;
	} else {
		shape->layers = 0;
	}
}

void Actor::goToGround() {
	// TODO: Make this function actually work again.  What it should do is move
	// the current actor down to where it is colliding with the ground.
}

void Actor::collideGround() {
	grounded++;
}

void Actor::leaveGround() {
//	grounded--;
}

void Actor::doUpdate(sf::Uint32 dt) {
	if(body == NULL) {
		update(dt);
	} else if(isOnCamera() || !canSleep) {
		if(!awake) {
			cpBodyActivate(body);
			awake = true;
		}
		update(dt);
		grounded = 0;
	} else  {
		if(awake) {
			awake = false;
			cpBodySleep(body);
		}
	}
	/*
	if(body && game_map) {
		sf::Vector2f pos = game_map->cp2sfml(body->p);
		setPos(pos.x, pos.y+height/2);
	}
	*/
}

void Actor::setLevel(int newLevel) {
	currentLevel = newLevel;
}

int Actor::getLevel() {
	return currentLevel;
}

void Actor::incrementLevel() {
	currentLevel++;
	onLevelUp(currentLevel);
}

int Actor::getExperienceValue() {
	return experienceValue;
}
	
void Actor::setExperienceValue(int exp) {
	experienceValue = exp;
}
	
void Actor::resetPhysics(double start_x, double start_y)
{
	// No map -> no physics
	if(!game_map || !game_map->physSpace) {
		return;
	}

	destroyPhysics();
	
	if(!staticBody) {
		body = cpSpaceAddBody(game_map->physSpace, cpBodyNew(10.0f, INFINITY));
		//body->p = game_map->sfml2cp(sf::Vector2f(pos_x, pos_y - height/2));
		body->p = cpv(start_x, start_y);

		shape = cpSpaceAddShape(game_map->physSpace, cpBoxShapeNew(body, width, height));
		shape->e = 0.0f; shape->u = 2.0f;
	} else {
		
		cpVect verts[] = {
			cpv(-width/2, -height/2),			
			cpv(-width/2, height/2),			
			cpv(width/2, height/2),
			cpv(width/2, -height/2)
		};
		
		//shape = cpSpaceAddShape(game_map->physSpace, cpPolyShapeNew(&game_map->physSpace->staticBody, 4, verts, game_map->sfml2cp(sf::Vector2f(pos_x, pos_y))));
		shape = cpSpaceAddShape(game_map->physSpace, cpPolyShapeNew(&game_map->physSpace->staticBody, 4, verts, cpv(start_x, start_y)));
		static_x = start_x;
		static_y = start_y;
	}
	
	shape->data = (void *) this;
	grounded = 0;
	resetPhysicsCustom(start_x, start_y);
}

void Actor::destroyPhysics() {
	//cout << "Destroying physics for: " << actorName << "\n";
	//cout << actorName << " destroy count " << ++destroyedCount << "\n";
	if(body != NULL) {
		if(shape) 
			cpSpaceRemoveShape(game_map->physSpace, shape);
		
		cpSpaceRemoveBody(game_map->physSpace, body);
		
		if(shape)
			cpShapeFree(shape);
		
		cpBodyFree(body);

	} else if(shape != NULL) {
		// Static things like collectibles have no body.
 		cpSpaceRemoveShape(game_map->physSpace, shape);
		cpShapeFree(shape);		
	}
	shape = NULL;
	body = NULL;
}

void Actor::setShapeLayers(cpLayers l) {
	shapeLayers = l;
	if(collideable) shape->layers = shapeLayers;
}

void Actor::freeze() {
	if(body) {
		//body->v = cpv(0, 0);
		body->velocity_func = no_gravity_stop;
	}
}

void Actor::unFreeze() {
	if(body) {
		body->velocity_func = defaultVelocityFunc;
	}
}

void no_gravity(struct cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt) {
	cpBodyUpdateVelocity(body, cpv(0, 0), 1, dt);
}

void no_gravity_stop(struct cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt) {
	cpBodyUpdateVelocity(body, cpv(0, 0), .1, dt);
}

void Actor::teleport(double x, double y, double vx, double vy) {
	toTeleport = true;
	teleport_x = x;
	teleport_y = y;
	teleport_vx = vx;
	teleport_vy = vy;
}

void Actor::doTeleport() {
	toTeleport = false;
	resetPhysics(teleport_x, teleport_y);
	if(body) {
		body->v.x = teleport_vx;
		body->v.y = teleport_vy;
	}
}
