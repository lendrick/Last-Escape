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

#include "ScriptedActor.h"

ScriptedActor::ScriptedActor(double x, double y, double w, double h, QString image, QString animation) :
	AnimatedActor(x, y, w, h, image.toStdString(), false)
{
	resetPhysics(x, y);
	//qDebug() << "Creating ScriptedActor";
	actorName = animation.toStdString();
	loadAnimationsFromFile(animation.toStdString());
	bodyWrapper = new PhysicsBodyScriptWrapper(body);
	shapeWrapper = new PhysicsShapeScriptWrapper(shape);
}

ScriptedActor::ScriptedActor(double x, double y, QString animation, bool staticBody) :
	AnimatedActor(x, y, animation.toStdString(), staticBody)
{
	resetPhysics(x, y);
	//qDebug() << "Creating ScriptedActor from animation " << animation;
	actorName = animation.toStdString();
	loadAnimationsFromFile(animation.toStdString());
	bodyWrapper = new PhysicsBodyScriptWrapper(body);
	shapeWrapper = new PhysicsShapeScriptWrapper(shape);
}

ScriptedActor::~ScriptedActor()
{
	qDebug() << "Deleting ScriptedActor " << actorName.data();
	delete bodyWrapper;
	delete shapeWrapper;
}

void ScriptedActor::setThisObject(QScriptValue thisObj) {
	thisObject = thisObj;
	thisObject.setProperty("body", bodyWrapper->thisObject);
	thisObject.setProperty("shape", shapeWrapper->thisObject);
}

void ScriptedActor::updateCallback(double dt)
{
	QScriptValue updateFunc = thisObject.property("update");
	if(updateFunc.isFunction()) {
		QScriptValue args = scriptEngine->newArray(1);
		args.setProperty(0, QScriptValue(dt));
		updateFunc.call(thisObject, QScriptValue(args));

		if(scriptEngine->hasUncaughtException()) {
			qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
									scriptEngine->uncaughtException().toString();
			scriptEngine->clearExceptions();
		}
	}
}

void ScriptedActor::die()
{
	QScriptValue updateFunc = thisObject.property("die");
	if(updateFunc.isFunction()) {
		updateFunc.call(thisObject, QScriptValue(QScriptValue::NullValue));

		if(scriptEngine->hasUncaughtException()) {
			qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
									scriptEngine->uncaughtException().toString();
			scriptEngine->clearExceptions();
		}
	} else {
		AnimatedActor::die();
	}
}

void ScriptedActor::setCurrentAnimation(QString name, bool reset)
{
	AnimatedActor::setCurrentAnimation(name.toStdString(), reset);
}

QString ScriptedActor::getCurrentAnimationName()
{
	return QString(currentAnimation->getName().data());
}

void ScriptedActor::resetCurrentAnimation(QString name)
{
	AnimatedActor::setCurrentAnimation(name.toStdString(), false);
}

void ScriptedActor::setFacing(int facing)
{
	facing_direction = facing;
}

int ScriptedActor::getFacing()
{
	return facing_direction;
}

void ScriptedActor::setCanSleep(bool cs)
{
	canSleep = cs;
}

bool ScriptedActor::getCanSleep()
{
	return canSleep;
}

PhysicsBodyScriptWrapper * ScriptedActor::getBodyWrapper()
{
	return bodyWrapper;
}

PhysicsShapeScriptWrapper * ScriptedActor::getShapeWrapper()
{
	return shapeWrapper;
}

QString ScriptedActor::getActorName()
{
	return QString(actorName.data());
}

void ScriptedActor::setActorName(QString n)
{
	actorName = n.toStdString();
}

void ScriptedActor::setLife(double l)
{
	life = l;
}

double ScriptedActor::getLife()
{
	return life;
}

void ScriptedActor::collideCallback(Actor& otherActor)
{
	QScriptValue updateFunc = thisObject.property("onCollide");
	QScriptValue args = scriptEngine->newArray(1);
	args.setProperty(0, QScriptValue(scriptEngine->newQObject(&otherActor, QScriptEngine::QtOwnership)));

	if(updateFunc.isFunction())
		updateFunc.call(thisObject, QScriptValue(args));

	if(scriptEngine->hasUncaughtException()) {
		qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
								scriptEngine->uncaughtException().toString();
		scriptEngine->clearExceptions();
	}
}

void ScriptedActor::bumperCollideCallback(int facing)
{
	//cout << "bumperCollideCallback " << facing << "\n";
	QScriptValue updateFunc = thisObject.property("onBumperCollide");
	if(updateFunc.isFunction()) {
		QScriptValue args = scriptEngine->newArray(1);
		args.setProperty(0, QScriptValue(facing));
		updateFunc.call(thisObject, QScriptValue(args));

		if(scriptEngine->hasUncaughtException()) {
			qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
									scriptEngine->uncaughtException().toString();
			scriptEngine->clearExceptions();
		}
	}
}

void ScriptedActor::bumperCollidingCallback(int facing)
{
	QScriptValue updateFunc = thisObject.property("onBumperColliding");
	if(updateFunc.isFunction()) {
		QScriptValue args = scriptEngine->newArray(1);
		args.setProperty(0, QScriptValue(facing));
		updateFunc.call(thisObject, QScriptValue(args));

		if(scriptEngine->hasUncaughtException()) {
			qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
									scriptEngine->uncaughtException().toString();
			scriptEngine->clearExceptions();
		}
	}
}

void ScriptedActor::bumperEndCollideCallback(int facing)
{
	QScriptValue updateFunc = thisObject.property("onEndBumperCollide");
	if(updateFunc.isFunction()) {
		QScriptValue args = scriptEngine->newArray(1);
		args.setProperty(0, QScriptValue(facing));
		updateFunc.call(thisObject, QScriptValue(args));

		if(scriptEngine->hasUncaughtException()) {
			qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
									scriptEngine->uncaughtException().toString();
			scriptEngine->clearExceptions();
		}
	}
}

void ScriptedActor::collidingCallback(Actor & otherActor)
{
	QScriptValue updateFunc = thisObject.property("onColliding");
	QScriptValue args = scriptEngine->newArray(1);
	args.setProperty(0, QScriptValue(scriptEngine->newQObject(&otherActor, QScriptEngine::QtOwnership)));

	if(updateFunc.isFunction())
		updateFunc.call(thisObject, QScriptValue(args));

	if(scriptEngine->hasUncaughtException()) {
		qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
								scriptEngine->uncaughtException().toString();
		scriptEngine->clearExceptions();
	}
}

void ScriptedActor::endCollideCallback(Actor & otherActor)
{
	QScriptValue updateFunc = thisObject.property("onEndCollide");
	QScriptValue args = scriptEngine->newArray(1);
	args.setProperty(0, QScriptValue(scriptEngine->newQObject(&otherActor, QScriptEngine::QtOwnership)));

	if(updateFunc.isFunction())
		updateFunc.call(thisObject, QScriptValue(args));

	if(scriptEngine->hasUncaughtException()) {
		qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
								scriptEngine->uncaughtException().toString();
		scriptEngine->clearExceptions();
	}
}

void ScriptedActor::destroyCallback()
{
	QScriptValue updateFunc = thisObject.property("onDestroy");
	if(updateFunc.isFunction())
		updateFunc.call(thisObject, QScriptValue(QScriptValue::NullValue));

	if(scriptEngine->hasUncaughtException()) {
		qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
								scriptEngine->uncaughtException().toString();
		scriptEngine->clearExceptions();
	}
}

void ScriptedActor::damageCallback()
{
	QScriptValue updateFunc = thisObject.property("onDamage");
	if(updateFunc.isFunction())
		updateFunc.call(thisObject, QScriptValue(QScriptValue::NullValue));

	if(scriptEngine->hasUncaughtException()) {
		qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
								scriptEngine->uncaughtException().toString();
		scriptEngine->clearExceptions();
	}
}

void ScriptedActor::animationCompleteCallback(std::string anim)
{
	QScriptValue updateFunc = thisObject.property("onAnimationComplete");

	if(updateFunc.isFunction()) {
		QScriptValue args = scriptEngine->newArray(1);
		args.setProperty(0, QScriptValue(anim.data()));

		updateFunc.call(thisObject, QScriptValue(args));

		if(scriptEngine->hasUncaughtException()) {
			qDebug() << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
									scriptEngine->uncaughtException().toString();
			scriptEngine->clearExceptions();
		}
	}
}

void ScriptedActor::setDamageTimer(double t)
{
	damageTimer = t;
}

double ScriptedActor::getDamageTimer()
{
	return damageTimer;
}

void ScriptedActor::setShapeLayers(int l)
{
	shapeLayers = l;

	if(collideable)
		shape->layers = l;
}

int ScriptedActor::getShapeLayers()
{
	return shapeLayers;
}

void ScriptedActor::setCollisionGroup(int g)
{
	collisionGroup = g;
	shape->group = g;
}

int ScriptedActor::getCollisionGroup()
{
	return collisionGroup;
}

void ScriptedActor::setCollisionType(int t)
{
	collisionType = t;
	shape->collision_type = t;
}

int ScriptedActor::getCollisionType()
{
	return shape->collision_type;
}

void ScriptedActor::setX(double x)
{
	body->p.x = x;
}

double ScriptedActor::getX()
{
	return body->p.x;
}

void ScriptedActor::setY(double y)
{
	body->p.y = y;
}

double ScriptedActor::getY()
{
	return body->p.y;
}

void ScriptedActor::setSensor(bool s)
{
	//cout << actorName << " sensor set to " << s << "\n";
	shape->sensor = s;
}

bool ScriptedActor::getSensor() {
	return shape->sensor;
}

QScriptValue scriptedActorConstructor(QScriptContext * context, QScriptEngine * engine)
{
	double x = context->argument(0).toNumber();
	double y = context->argument(1).toNumber();
	QString anim = context->argument(2).toString();
	try {
		ScriptedActor * object = new ScriptedActor(x, y, anim);
		QScriptValue thisObject = engine->newQObject(object, QScriptEngine::QtOwnership);
		object->setThisObject(thisObject);
		return thisObject;
	} catch(QString s) {
		return context->throwError(s);
	}
}

QScriptValue scriptedStaticActorConstructor(QScriptContext * context, QScriptEngine * engine)
{
	double x = context->argument(0).toNumber();
	double y = context->argument(1).toNumber();
	QString anim = context->argument(2).toString();
	try {
		ScriptedActor * object = new ScriptedActor(x, y, anim, true);
		QScriptValue thisObject = engine->newQObject(object, QScriptEngine::QtOwnership);
		object->setThisObject(thisObject);
		return thisObject;
	} catch(QString s) {
		return context->throwError(s);
	}
}

