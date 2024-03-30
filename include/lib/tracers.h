#pragma once
#include "pros/adi.hpp"

namespace lib {

class Tracer {

private:
    pros::adi::Encoder* sensor;
    double wheelDiameter;
    double offset;
    double gearRatio;

public:
  Tracer(int port, double wheelDiameter, double offset,
         double gearRatio = 1);

    void reset();
    double getDistancetraveled();
    double getOffset();

};

} // namespace lib