#include "lib/chassis.h"
#include "pros/rtos.hpp"
#include <cstdint>
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

  //print out the profile to terminal without text
  for (std::pair<double, double> point : profile) {
    printf("%f, %f\n", point.first, point.second);}

  pros::lcd::set_text(3, "Hello PROS User!");
  for (std::pair<double, double> point : profile) {
    int target = controller->step(point.second, point.first, motors.getDiffyPos()[0], motors.getDiffyVel()[0]);
    motors.spinDiffy(target, target);

    pros::Task::delay_until(&now, 15);
  }
  pros::lcd::set_text(4, "Hello PROS User!");
}