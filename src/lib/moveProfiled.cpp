#include "lib/chassis.h"
#include "pros/rtos.hpp"
#include <cstdint>
#include <math.h>
#include <string>
#include "pros/llemu.hpp"

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

  leftMotors->tare_position_all();
  rightMotors->tare_position_all();

  //print out a line of space
  printf("\n");
  for (std::pair<double, double> point : profile) {
    leftMotors->move_velocity(point.first/constraints.max_velocity * 200);
    rightMotors->move_velocity(point.first/constraints.max_velocity * 200);
    //print velocities from motors
    printf("%f, %f, %f\n",point.first, point.second, leftMotors->get_actual_velocity()/200 * constraints.max_velocity);

    pros::Task::delay_until(&now, 15);
  }
  //print converted value back from ticks to inches using tpr and wheel
  pros::lcd::set_text(4, std::to_string(leftMotors->get_position(0)/ 900 * 3.25 * M_PI));
}