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

#include "globals.h"
#include "PhysicsBodyScriptWrapper.h"
#include "PhysicsVectorScriptWrapper.h"

PhysicsBodyScriptWrapper::PhysicsBodyScriptWrapper(cpBody * b) :
	QObject(0)
{
	body = b;
	thisObject = scriptEngine->newQObject(this, QScriptEngine::QtOwnership);

	velocityWrapper = new PhysicsVectorScriptWrapper(&body->v);
	velocityValue = scriptEngine->newQObject(velocityWrapper, QScriptEngine::QtOwnership);
	thisObject.setProperty("velocity", velocityValue);

	positionWrapper = new PhysicsVectorScriptWrapper(&body->p);
	positionValue = scriptEngine->newQObject(positionWrapper, QScriptEngine::QtOwnership);
	thisObject.setProperty("position", positionValue);

	forceWrapper = new PhysicsVectorScriptWrapper(&body->f);
	forceValue = scriptEngine->newQObject(forceWrapper, QScriptEngine::QtOwnership);
	thisObject.setProperty("force", forceValue);

	rotationWrapper = new PhysicsConstVectorScriptWrapper(&body->rot);
	rotationValue = scriptEngine->newQObject(rotationWrapper, QScriptEngine::QtOwnership);
	thisObject.setProperty("rotationVector", rotationValue);
}

PhysicsBodyScriptWrapper::~PhysicsBodyScriptWrapper()
{
	qDebug() << "deleting body script wrapper";
	delete velocityWrapper;
	delete positionWrapper;
	delete forceWrapper;
	delete rotationWrapper;
}

PhysicsVectorScriptWrapper * PhysicsBodyScriptWrapper::getVelocity()
{
	return velocityWrapper;
}

PhysicsVectorScriptWrapper * PhysicsBodyScriptWrapper::getPosition()
{
	return positionWrapper;
}

PhysicsVectorScriptWrapper * PhysicsBodyScriptWrapper::getForce()
{
	return forceWrapper;
}

PhysicsConstVectorScriptWrapper * PhysicsBodyScriptWrapper::getRotationVector()
{
	return rotationWrapper;
}

double PhysicsBodyScriptWrapper::getMass()
{
	return cpBodyGetMass(body);
}

void PhysicsBodyScriptWrapper::setMass(double m)
{
	cpBodySetMass(body, m);
}

double PhysicsBodyScriptWrapper::getMoment()
{
	return cpBodyGetMoment(body);
}

void PhysicsBodyScriptWrapper::setMoment(double m)
{
	cpBodySetMoment(body, m);
}

double PhysicsBodyScriptWrapper::getRotation()
{
	return cpBodyGetAngle(body);
}

void PhysicsBodyScriptWrapper::setRotation(double r)
{
	cpBodySetAngle(body, r);
}

double PhysicsBodyScriptWrapper::getRotationDegrees()
{
	return rad2deg(cpBodyGetAngle(body));
}

void PhysicsBodyScriptWrapper::setRotationDegrees(double r)
{
	cpBodySetAngle(body, deg2rad(r));
}

double PhysicsBodyScriptWrapper::getRotationalVelocity()
{
	return cpBodyGetAngVel(body);
}

void PhysicsBodyScriptWrapper::setRotationalVelocity(double v)
{
	cpBodySetAngVel(body, v);
}

double PhysicsBodyScriptWrapper::getTorque()
{
	return cpBodyGetTorque(body);
}

void PhysicsBodyScriptWrapper::setTorque(double t)
{
	cpBodySetTorque(body, t);
}

double PhysicsBodyScriptWrapper::getVelocityLimit()
{
	return cpBodyGetVelLimit(body);
}

void PhysicsBodyScriptWrapper::setVelocityLimit(double l)
{
	cpBodySetVelLimit(body, l);
}

double PhysicsBodyScriptWrapper::getRotationalVelocityLimit()
{
	return cpBodyGetAngVelLimit(body);
}

void PhysicsBodyScriptWrapper::setRotationalVelocityLimit(double l)
{
	cpBodySetAngVelLimit(body, l);
}

void PhysicsBodyScriptWrapper::applyImpulse(double fx, double fy, double offsetX, double offsetY)
{
	cpBodyApplyImpulse(body, cpv(fx, fy), cpv(offsetX, offsetY));
}
