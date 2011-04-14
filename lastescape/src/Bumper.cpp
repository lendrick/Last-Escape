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
#include "ScriptedActor.h"
#include "PhysicsShapeScriptWrapper.h"

Bumper::Bumper(Actor * actor, int facing_direction, double thickness) : QObject(0)
{

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
	} else {
		cout << "Could not create bumper: invalid facing direction\n";
		throw("Invalid direction");
	}

  shape->data = (void *) this;
  shape->sensor = true;
  shape->layers = PhysicsLayer::Map|PhysicsLayer::Enemy;
  shape->collision_type = PhysicsType::Bumper;
  shape->group = PhysicsGroup::Bumpers;
  grounded = 0;

	// The bumper should be deleted along with its parent.

	thisObject = scriptEngine->newQObject(this, QScriptEngine::QtOwnership);

	scriptedActor = dynamic_cast<ScriptedActor *>(this->actor);
	if(scriptedActor) {
		actorValue = scriptEngine->newQObject(scriptedActor, QScriptEngine::QtOwnership);
		thisObject.setProperty("actor", actorValue);
	}

	shapeWrapper = new PhysicsShapeScriptWrapper(shape);
	shapeValue = scriptEngine->newQObject(shapeWrapper, QScriptEngine::QtOwnership);
	thisObject.setProperty("shape", shapeValue);

}

Bumper::~Bumper()
{
	//cout << "Deleting bumper on " << actor->actorName << "\n";
  cpSpaceRemoveShape(game_map->physSpace, shape);
  cpShapeFree(shape);
}

void Bumper::setThisObject(QScriptValue o) {
	thisObject = o;
	scriptedActor = dynamic_cast<ScriptedActor *>(this->actor);
	if(scriptedActor) {
		//cout << "Scripted actor " << scriptedActor->actorName << "\n";
		//actorValue = scriptEngine->newQObject(scriptedActor, QScriptEngine::QtOwnership);
		thisObject.setProperty("actor", scriptedActor->thisObject);
	} else {
		cout << "No scripted actor " << this->actor->actorName << "\n";
	}

	shapeWrapper = new PhysicsShapeScriptWrapper(shape);
	shapeValue = scriptEngine->newQObject(shapeWrapper, QScriptEngine::QtOwnership);
	thisObject.setProperty("shape", shapeValue);

	//if(scriptedActor) cout << "New bumper on " << actor->actorName << " " << actor->width << " " << actor->height << "  (" << facing_direction << ")\n";
	//cout << shape->body->p.x << " " << shape->body->p.y << "\n";
}

bool Bumper::isGrounded()
{
  return grounded > 0;
}

void Bumper::collideGround()
{
	//if(scriptedActor) cout << "Bumper on ground\n";
  grounded++;
}

void Bumper::leaveGround()
{
	//if(scriptedActor) cout << "Bumper off of ground\n";
  grounded--;
}

int Bumper::getFacingDirection()
{
	return facing_direction;
}


QScriptValue bumperConstructor(QScriptContext * context, QScriptEngine *engine)
{
	//cout << "bumperConstructon\n";
	Actor * actor = dynamic_cast<Actor *>(context->argument(0).toQObject());

	int facing = context->argument(1).toInteger();
	double thickness = context->argument(2).toNumber();
	if(thickness == 0) thickness = 4.0;

	if(actor) {
		//cout << "actor " << actor->actorName << " facing " << facing << "\n";
		try {
			Bumper * object = new Bumper(actor, facing, thickness);
			//QScriptValue thisObject = engine->newQObject(object, QScriptEngine::ScriptOwnership);
			//object->setThisObject(thisObject);
			return object->thisObject;
		} catch(QString s) {
			return context->throwError(s);
		}
	} else {
		return context->throwError("Tried to attach a bumper to a non-actor.");
	}
}
