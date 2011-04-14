#include "Sensor.h"

Sensor::Sensor(double x, double y, double w, double h)
:Actor(x, y, w, h, true) {
  shape->sensor = true;
	shape->collision_type = PhysicsType::Sensor;
	shape->layers = PhysicsLayer::Player;
	actorName = "Sensor";
}
