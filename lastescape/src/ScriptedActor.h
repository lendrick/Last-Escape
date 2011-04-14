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
#include <QtScript>
#include "AnimatedActor.h"
#include "PhysicsShapeScriptWrapper.h"
#include "PhysicsBodyScriptWrapper.h"

class ScriptedActor : public AnimatedActor
{
  Q_OBJECT
	Q_PROPERTY(QString animation READ getCurrentAnimationName WRITE resetCurrentAnimation)
	Q_PROPERTY(int facing READ getFacing WRITE setFacing)
	Q_PROPERTY(bool canSleep READ getCanSleep WRITE setCanSleep)
	Q_PROPERTY(QString actorName READ getActorName WRITE setActorName)
	Q_PROPERTY(double life READ getLife WRITE setLife)
	Q_PROPERTY(double damageTimer READ getDamageTimer WRITE setDamageTimer)
	Q_PROPERTY(int shapeLayers READ getShapeLayers WRITE setShapeLayers)
	Q_PROPERTY(int collisionGroup READ getCollisionGroup WRITE setCollisionGroup)
	Q_PROPERTY(int collisionType READ getCollisionType WRITE setCollisionType)
	Q_PROPERTY(double x READ getX WRITE setX)
	Q_PROPERTY(double y READ getY WRITE setY)
	Q_PROPERTY(bool sensor READ getSensor WRITE setSensor)
	Q_PROPERTY(int experienceValue READ getExperienceValue WRITE setExperienceValue)

public:
	explicit ScriptedActor(double x, double y, double w, double h, QString image, QString animation);
	explicit ScriptedActor(double x, double y, QString filename);
	virtual ~ScriptedActor();
	void setThisObject(QScriptValue thisObj);

	virtual void update(double dt);
	virtual void die();

	virtual void collideCallback(Actor& otherActor);
	virtual void bumperCollideCallback(int facing);
	virtual void bumperCollidingCallback(int facing);
	virtual void bumperEndCollideCallback(int facing);
	virtual void collidingCallback(Actor & otherActor);
	virtual void endCollideCallback(Actor & otherActor);
	virtual void destroyCallback();
	virtual void damageCallback();

	virtual void animationCompleteCallback(std::string anim);


	PhysicsBodyScriptWrapper * getBodyWrapper();
	PhysicsShapeScriptWrapper * getShapeWrapper();

	QScriptValue thisObject;
signals:

public slots:
	QString getCurrentAnimationName();
	void resetCurrentAnimation(QString name);
	void setCurrentAnimation(QString name, bool reset = true);

	void setFacing(int facing);
	int getFacing();

	void setCanSleep(bool cs);
	bool getCanSleep();

	QString getActorName();
	void setActorName(QString n);

	void setLife(double l);
	double getLife();

	void setDamageTimer(double t);
	double getDamageTimer();

	void setShapeLayers(int l);
	int getShapeLayers();

	void setCollisionGroup(int g);
	int getCollisionGroup();

	void setCollisionType(int t);
	int getCollisionType();

	void setX(double x);
	double getX();

	void setY(double y);
	double getY();

	void setSensor(bool s);
	bool getSensor();
private:
	QScriptValue bw;
	PhysicsBodyScriptWrapper * bodyWrapper;
	PhysicsShapeScriptWrapper * shapeWrapper;
};

QScriptValue scriptedActorConstructor(QScriptContext * context, QScriptEngine * engine);

ScriptedActor * loadScriptedActor(QString filename);
