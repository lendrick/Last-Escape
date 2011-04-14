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

#include <QtScript>
#include "Utils.h"
#include "globals.h"
#include "Map.h"
#include "SoundCache.h"


/**
 * trim: remove leading and trailing c from s
 */
string trim(string s, char c)
{
  if (s.length() == 0) return "";

  unsigned int first = 0;
  unsigned int last = s.length()-1;

  while (s.at(first) == c && first < s.length()-1) {
    first++;
  }
  while (s.at(last) == c && last >= first) {
    last--;
  }
  if (first <= last) return s.substr(first,last-first+1);
  return "";
}

/**
 * parse "[val]"
 */
string parse_section_title(string s)
{
  size_t bracket = s.find_first_of(']');
  if (bracket == string::npos) return ""; // not found
  return s.substr(1, bracket-1);
}

/**
 * parse "key=val"
 */
void parse_key_pair(string s, string &key, string &val)
{
  size_t separator = s.find_first_of('=');
  if (separator == string::npos) {
    key = "";
    val = "";
    return; // not found
  }
  key = s.substr(0, separator);
  val = s.substr(separator+1, s.length());
}

cpShape * boxWithOffset(cpBody * body, double width, double height, sf::Vector2f offset)
{
  return boxWithOffset(body, width, height, game_map->sfml2cp(offset));
}

cpShape * boxWithOffset(cpBody * body, double width, double height, cpVect offset)
{
  //cout << "new box with offset " << width << "x" << height << "(" << offset.x << ", " << offset.y << ")\n";
  cpVect verts[] = {
    cpv(-width/2, -height/2),
    cpv(-width/2, height/2),
    cpv(width/2, height/2),
    cpv(width/2, -height/2)
  };

  return cpPolyShapeNew(body, 4, verts, offset);
}

double clamp(double n, double min, double max)
{
  if(max < min) {
    double tmp;
    tmp = max;
    max = min;
    min = tmp;
  }
  if(n > max) n = max;
  if(n < min) n = min;
  return n;
}

QScriptValue runScript(QString filename) {
	//cprint("evaluating file '" + filename + "'");

	filename = "scripts/" + filename;
	if(debugMode)
		cout << "evaluating script '" << filename.toStdString() << "'\n";

	QFile f(filename);
	f.open(QIODevice::ReadOnly);

	if(!f.exists()) {
		//message("File '" + filename +"' not found");
		return QScriptValue();
	}

	QTextStream in(&f);
	QString program(in.readAll());
	f.close();

	QScriptContext *context = scriptEngine->currentContext();
	QScriptContext *parent = context->parentContext();
	if(parent!=0)
	{
		context->setActivationObject(context->parentContext()->activationObject());
		context->setThisObject(context->parentContext()->thisObject());
	}

	// Change to the directory of the script before running it.
	/*
	QFileInfo fileInfo(filename);
	QDir currentDir = QDir::current();
	QDir::setCurrent(fileInfo.absolutePath());
	*/
	//qDebug() << "DIR:" << QDir::currentPath();

	// Run the script
	QScriptValue r = scriptEngine->evaluate(program, filename);

	// Change back to the original directory when finished.
	//QDir::setCurrent(currentDir.absolutePath());

	if(scriptEngine->hasUncaughtException()) {
		//cprint(filename + ": " + QString::number(scriptEngine->uncaughtExceptionLineNumber()) + ": " +
		//				scriptEngine->uncaughtException().toString());
		qDebug() << filename << ": " << QString::number(scriptEngine->uncaughtExceptionLineNumber()) << ": " <<
								scriptEngine->uncaughtException().toString();
		scriptEngine->clearExceptions();
	}

	return r;
}
