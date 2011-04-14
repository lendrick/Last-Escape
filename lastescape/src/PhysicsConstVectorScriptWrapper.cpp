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

#include <QtCore>
#include "PhysicsConstVectorScriptWrapper.h"

PhysicsConstVectorScriptWrapper::PhysicsConstVectorScriptWrapper(cpVect *vect) :
	QObject(0)
{
	vector = vect;
}

PhysicsConstVectorScriptWrapper::PhysicsConstVectorScriptWrapper() :
		QObject(0)
{
	vector = NULL;
}

PhysicsConstVectorScriptWrapper::~PhysicsConstVectorScriptWrapper()
{
	qDebug() << "deleting const vector wrapper";
}

double PhysicsConstVectorScriptWrapper::getX()
{
	return vector->x;
}

double PhysicsConstVectorScriptWrapper::getY()
{
	return vector->y;
}

void PhysicsConstVectorScriptWrapper::dump()
{
	qDebug() << vector->x << ", " << vector->y;
}
