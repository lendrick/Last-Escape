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
#include "PhysicsBodyScriptWrapper.h"

class PhysicsShapeScriptWrapper : public QObject
{
	Q_OBJECT
	Q_PROPERTY(double elasticity READ getElasticity WRITE setElasticity)
	Q_PROPERTY(double friction READ getFriction WRITE setFriction)
	Q_PROPERTY(int collisionType READ getCollisionType WRITE setCollisionType)
	Q_PROPERTY(int group READ getGroup WRITE setGroup)
	Q_PROPERTY(int layers READ getLayers WRITE setLayers)
	Q_PROPERTY(bool sensor READ isSensor WRITE setSensor)
public:
	explicit PhysicsShapeScriptWrapper(cpShape * s);
	~PhysicsShapeScriptWrapper();
	cpShape * shape;

	QScriptValue thisObject;
	QScriptValue surfaceVelocityValue;
	QScriptValue bodyValue;

	PhysicsVectorScriptWrapper * surfaceVelocityWrapper;
	PhysicsBodyScriptWrapper * bodyWrapper;
signals:

public slots:
	void setElasticity(double e);
	double getElasticity();

	void setFriction(double f);
	double getFriction();

	void setCollisionType(int t);
	int getCollisionType();

	void setGroup(int g);
	int getGroup();

	void setLayers(int l);
	int getLayers();

	void setSensor(bool s);
	bool isSensor();

	PhysicsBodyScriptWrapper * getBody();
	PhysicsVectorScriptWrapper * getSurfaceVelocity();
};

