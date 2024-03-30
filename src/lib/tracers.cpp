#include "lib/tracers.h"
#include "pros/adi.hpp"
#include <cmath>

using namespace lib;

Tracer::Tracer(int port, double wheelDiameter, double offset,
               double gearRatio) {
  this->wheelDiameter = wheelDiameter;
  this->offset = offset;
  this->gearRatio = gearRatio;

  auto temp = pros::adi::Encoder(std::abs(port), std::abs(port) + 1,
                                 port < 0 ? true : false);
  sensor = &temp;
}

void Tracer::reset() { sensor->reset(); }

double Tracer::getDistancetraveled() {
  return sensor->get_value() * (wheelDiameter * M_PI) / gearRatio;
}

double Tracer::getOffset() { return offset; }