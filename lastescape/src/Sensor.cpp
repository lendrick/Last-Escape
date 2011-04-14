#include "Sensor.h"

Sensor::Sensor(double x, double y, double w, double h)
  :Actor(x, y, w, h, true)
{
	collisionType = PhysicsType::Sensor;
	shapeLayers = PhysicsLayer::Player;
	resetPhysics(x, y);
	shape->sensor = true;
  actorName = "Sensor";
}
