/*
 *  This file is part of Last Escape.
 *
 *  Last Escape is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Last Escape.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "PhysicsShapeScriptWrapper.h"
#include "globals.h"

PhysicsShapeScriptWrapper::PhysicsShapeScriptWrapper(cpShape * sh) :
	QObject()
{
	shape = sh;
	thisObject = scriptEngine->newQObject(this, QScriptEngine::QtOwnership);

	surfaceVelocityWrapper = new PhysicsVectorScriptWrapper(&shape->surface_v);
	surfaceVelocityValue = scriptEngine->newQObject(surfaceVelocityWrapper, QScriptEngine::QtOwnership);
	thisObject.setProperty("surfaceVelocity", surfaceVelocityValue);

	bodyWrapper = new PhysicsBodyScriptWrapper(shape->body);
	bodyValue = scriptEngine->newQObject(bodyWrapper, QScriptEngine::QtOwnership);
	thisObject.setProperty("body", bodyValue);
}

PhysicsShapeScriptWrapper::~PhysicsShapeScriptWrapper()
{
	qDebug() << "Deleting shape wrapper";
	delete surfaceVelocityWrapper;
	delete bodyWrapper;
}

void PhysicsShapeScriptWrapper::setElasticity(double e)
{
	shape->e = e;
}

double PhysicsShapeScriptWrapper::getElasticity()
{
	return shape->e;
}

void PhysicsShapeScriptWrapper::setFriction(double f)
{
	shape->u = f;
}

double PhysicsShapeScriptWrapper::getFriction()
{
	return shape->u;
}

void PhysicsShapeScriptWrapper::setCollisionType(int t)
{
	shape->collision_type = t;
}

int PhysicsShapeScriptWrapper::getCollisionType()
{
	return shape->collision_type;
}

void PhysicsShapeScriptWrapper::setGroup(int g)
{
	shape->group = g;
}

int PhysicsShapeScriptWrapper::getGroup()
{
	return shape->group;
}

void PhysicsShapeScriptWrapper::setLayers(int l)
{
	shape->layers = l;
}

int PhysicsShapeScriptWrapper::getLayers()
{
	return shape->layers;
}

void PhysicsShapeScriptWrapper::setSensor(bool s)
{
	shape->sensor = s;
}

bool PhysicsShapeScriptWrapper::isSensor()
{
	return shape->sensor;
}

PhysicsBodyScriptWrapper * PhysicsShapeScriptWrapper::getBody()
{
	return bodyWrapper;
}

PhysicsVectorScriptWrapper * PhysicsShapeScriptWrapper::getSurfaceVelocity()
{
	return surfaceVelocityWrapper;
}

