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

#pragma once
#include <QObject>
#include <QtScript>
#include <chipmunk/chipmunk.h>
#include "PhysicsVectorScriptWrapper.h"
#include "PhysicsConstVectorScriptWrapper.h"

class PhysicsBodyScriptWrapper : public QObject
{
	Q_OBJECT
	Q_PROPERTY(double mass READ getMass WRITE setMass)
	Q_PROPERTY(double moment READ getMoment WRITE setMoment)
	Q_PROPERTY(double rotation READ getRotation WRITE setRotation)
	Q_PROPERTY(double rotationDegrees READ getRotationDegrees WRITE setRotationDegrees)
	Q_PROPERTY(double rotationalVelocity READ getRotationalVelocity WRITE setRotationalVelocity)
	Q_PROPERTY(double torque READ getTorque WRITE setTorque)
	Q_PROPERTY(double velocityLimit READ getVelocityLimit WRITE setVelocityLimit)
	Q_PROPERTY(double rotationalVelocityLimit READ getRotationalVelocityLimit WRITE setRotationalVelocityLimit)
public:
	explicit PhysicsBodyScriptWrapper(cpBody * b);
	~PhysicsBodyScriptWrapper();

	cpBody * body;
	PhysicsVectorScriptWrapper * velocityWrapper;
	PhysicsVectorScriptWrapper * positionWrapper;
	PhysicsVectorScriptWrapper * forceWrapper;
	PhysicsConstVectorScriptWrapper * rotationWrapper;

	QScriptValue thisObject;

	QScriptValue velocityValue;
	QScriptValue positionValue;
	QScriptValue forceValue;
	QScriptValue rotationValue;
signals:

public slots:
	PhysicsVectorScriptWrapper * getVelocity();
	PhysicsVectorScriptWrapper * getPosition();
	PhysicsVectorScriptWrapper * getForce();
	PhysicsConstVectorScriptWrapper * getRotationVector();

	double getMass();
	void setMass(double m);

	double getMoment();
	void setMoment(double m);

	double getRotation();
	void setRotation(double r);

	double getRotationDegrees();
	void setRotationDegrees(double r);

	double getRotationalVelocity();
	void setRotationalVelocity(double v);

	double getTorque();
	void setTorque(double t);

	double getVelocityLimit();
	void setVelocityLimit(double l);

	double getRotationalVelocityLimit();
	void setRotationalVelocityLimit(double l);

	void applyImpulse(double fx, double fy, double offsetX = 0.0, double offsetY = 0.0);
};

