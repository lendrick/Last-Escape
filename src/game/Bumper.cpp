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

#include "Bumper.h"
#include "Utils.h"
#include "Actor.h"
#include "Map.h"

Bumper::Bumper(Actor * actor, int facing_direction, double thickness) {
	//cout << "New bumper on " << actor->actorName << " " << actor->width << " " << actor->height << "  " << facing_direction << "\n";
	cpBody * body = actor->body;
	this->actor = actor;
	this->facing_direction = facing_direction;
	
	if(facing_direction == Facing::Left) {
		shape = cpSpaceAddShape(
			game_map->physSpace, 
			boxWithOffset(body, thickness, actor->height, cpv(-actor->width/2 - 1, 0))
		);
	} else if(facing_direction == Facing::Right) {
		shape = cpSpaceAddShape(
			game_map->physSpace, 
			boxWithOffset(body, thickness, actor->height, cpv(actor->width/2 + 1, 0))
		);
	} else if(facing_direction == Facing::Down) {
		shape = cpSpaceAddShape(
			game_map->physSpace, 
			boxWithOffset(body, actor->width, thickness, cpv(0, -actor->height/2 - 1))
		);
	} else if(facing_direction == Facing::Up) {
		shape = cpSpaceAddShape(
			game_map->physSpace, 
			boxWithOffset(body, actor->width, thickness, cpv(0, actor->height/2 + 1))
		);
	}
	
	shape->data = (void *) this;
	shape->sensor = true;
	shape->layers = PhysicsLayer::Map|PhysicsLayer::Enemy;
	shape->collision_type = PhysicsType::Bumper;
	shape->group = PhysicsGroup::Bumpers;
	grounded = 0;
}

Bumper::~Bumper() {
	cpSpaceRemoveShape(game_map->physSpace, shape);
	cpShapeFree(shape);		
}

int Bumper::isGrounded() {
	return grounded > 0;
}

void Bumper::collideGround() {
	//cout << "Bumper on ground\n";
	grounded++;
}

void Bumper::leaveGround() {
	//cout << "Bumper off of ground\n";
	grounded--;
}