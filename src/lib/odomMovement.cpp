#include "lib/chassis.h"
#include "point.hpp"
#include "pros/rtos.hpp"
#include "util.h"
#include <cmath>
#include <cstdlib>

using namespace lib;

void Chassis::moveToPoint(float x, float y, PID linearPid, PID turningPid,
                          float maxSpeed, bool async) {

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task(
        [&]() { moveToPoint(x, y, linearPid, turningPid, maxSpeed); });
  }

  state = DriveState::MOVING;

  // turn to the point
  double angle = atan2(y - currentPose.y, x - currentPose.x);
  turn(angle, turningPid, maxSpeed, false);

  // move to the point
  float distance = sqrt(pow(x - currentPose.x, 2) + pow(y - currentPose.y, 2));
  move(distance, linearPid, turningPid, maxSpeed, false);

  state = DriveState::IDLE;
}