#include "lib/chassis.h"
#include "point.hpp"
#include "pros/rtos.hpp"
#include "util.h"

using namespace lib;

void Chassis::moveToPoint(float x, float y, PID linearPid, PID turningPid,
                          bool backwards, float maxSpeed, bool async) {

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() {
      moveToPoint(x, y, linearPid, turningPid, backwards, maxSpeed);
    });
  }

  state = DriveState::MOVING;

  // turn to the point
  double angle = fmod(
      radiansToDegrees(M_PI_2 - atan2(y - getPose().y, x - getPose().x)), 360);
  std::cout << angle << "\n";

  if (backwards) {
    angle += 180;
  }
  turn(angle, turningPid, maxSpeed, false);

  // move to the point
  float distance = sqrt(pow(x - getPose().x, 2) + pow(y - getPose().y, 2));
  std::cout << distance << "\n";

  if (backwards) {
    distance = -distance;
  }
  move(distance, linearPid, turningPid, maxSpeed, false);

  state = DriveState::IDLE;
}