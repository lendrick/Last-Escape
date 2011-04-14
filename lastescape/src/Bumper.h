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

#pragma once
#include "globals.h"
#include <chipmunk/chipmunk.h>
#include <QtCore>
#include <QtScript>

class Actor;
class PhysicsShapeScriptWrapper;
class ScriptedActor;

class Bumper : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int facing READ getFacingDirection)
	Q_PROPERTY(bool grounded READ isGrounded)
public:
  Bumper(Actor * actor, int facing_direction, double thickness = 4.0f);
  ~Bumper();
	void setThisObject(QScriptValue o);
	Actor * actor;
	ScriptedActor * scriptedActor;
	cpShape * shape;
	int facing_direction;
	int grounded;

	PhysicsShapeScriptWrapper * shapeWrapper;
	QScriptValue thisObject;
	QScriptValue actorValue;
	QScriptValue shapeValue;

public slots:
	bool isGrounded();
  void collideGround();
  void leaveGround();
	int getFacingDirection();
};

QScriptValue bumperConstructor(QScriptContext * context, QScriptEngine *);
