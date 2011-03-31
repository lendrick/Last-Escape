#include "Sensor.h"

Sensor::Sensor(float x, float y, float w, float h)
:Actor(x, y, w, h, true) {
  shape->sensor = true;
}
