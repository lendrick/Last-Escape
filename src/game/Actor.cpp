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

Actor::Actor(float x, float y, float w, float h, bool staticBody) {
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
	resetPhysics(x, y);
}

Actor::~Actor() {
	//cout << "delete actor " << actorName << "\n";
	destroyPhysics();
}

void Actor::setPlaceholder(sf::Color c, float w, float h, float xoff, float yoff) {
	width = (int)w;
	height = (int)h;
	sprite.SetColor(c);
	sprite.SetScale((float)width, (float)height);
	sprite.SetCenter(xoff, yoff);
}

void Actor::setVelocityFunc(cpBodyVelocityFunc f) {
	if(body)
		body->velocity_func = f;
	defaultVelocityFunc = f;
}

void Actor::setDrawOffset(int ox, int oy) {
	xDrawOffset = ox;
	yDrawOffset = oy;
	sprite.SetCenter((float)ox, (float)oy);
}

void Actor::getPos(float &px, float &py) {
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

void Actor::draw() {
	if(!hidden && hasImage) {
		cpVect pos;
		int pos_x, pos_y;

		sf::Vector2f cam = game_map->cp2sfml(cpv(game_map->cam_x, game_map->cam_y));
		
		if(body) {
			pos = cpv(body->p.x, body->p.y + height/2);
		} else {
			pos = cpv(static_x, static_y + height/2);
		}
		
		if(body && body->a) {
			sprite.SetRotation(rad2deg(body->a));
		} else {
			sprite.SetRotation(0);
		}
		
		sf::Vector2f sfPos = game_map->cp2sfml(pos);
		
		/*
		if(isPlayer()) {
			if(body) {
				cout << "Position from physics body\n";
			} else {
				cout << "Position from static shape\n";
			}
			cout << "Player at " << body->p.x << " " << body->p.y << "\n";
			cout << "Chipmunk screen position " << pos.x << " " << pos.y << "\n";
			cout << "SFML Camera position " << cam.x << " " << cam.y << "\n";
			cout << "SFML screen position " << sfPos.x - cam.x << " " << sfPos.y - cam.y<< "\n\n";
		}
		*/
		
		//sprite.SetPosition(
			//0.5f + (int)(sfPos.x - cam.x),
			//0.5f + (int)(sfPos.y - cam.y));
		//sprite.SetCenter(-width/2, height/2);
		sprite.FlipY(true);
		if(body) {
			sprite.SetPosition(body->p.x, body->p.y + height);
		} else {
			sprite.SetPosition(static_x, static_y);
		}
		App->Draw(sprite);
		
		if(debugMode)
        {
						float px, py;
						getPos(px, py);
						
            float bbx1, bby1, bbx2, bby2;
            bbx1 = px - width/2;
            bby1 = py - height/2;
            bbx2 = bbx1 + width;
            bby2 = bby1 + height;
						
						sf::Shape rect = sf::Shape::Rectangle(-width/2, -height/2, width/2, height/2,
                                           sf::Color(0, 0, 0, 0), 1.0f, sf::Color(255, 0, 0));
						rect.SetPosition(px, py);
						if(body && body->a) {
							rect.SetRotation(rad2deg(body->a));
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
	grounded--;
}

void Actor::doUpdate(float dt) {
	update(dt);
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
	
void Actor::resetPhysics(float start_x, float start_y)
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
}

void Actor::destroyPhysics() {
	if(body) {
		if(shape) 
			cpSpaceRemoveShape(game_map->physSpace, shape);
		
		cpSpaceRemoveBody(game_map->physSpace, body);
		
		if(shape)
			cpShapeFree(shape);
		
		cpBodyFree(body);

	} else if(shape) {
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