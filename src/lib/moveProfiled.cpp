#include "lib/chassis.h"
#include "pros/rtos.hpp"
#include <cstdint>

using namespace lib;

void Chassis::moveProfiled(double target, profileConstraints constraints,
                           bool async) {
  Profiler *profiler =
      new Profiler(constraints.max_velocity, constraints.max_acceleration,
                   constraints.max_deceleration);
  std::vector<std::pair<double, double>> profile =
      profiler->generateProfile(target);

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }

    pros::Task task([&]() { moveProfiled(target, constraints, false); });
  }

  uint32_t now = pros::millis();

  state = DriveState::MOVING;

  for (std::pair<double, double> point : profile) {
    motors->spinVelocity(point.first, point.first);

    pros::Task::delay_until(&now, 10);
  }
}