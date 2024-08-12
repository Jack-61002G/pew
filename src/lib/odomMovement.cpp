#include "lib/chassis.h"
#include "point.hpp"
#include "util.h"
#include <cmath>
#include <cstdlib>

using namespace lib;



void Chassis::moveToPoint(float x, float y, PID linearPid, PID headingPid, int timeout, float maxSpeed, bool async) {
  const float largeError = 1;
  const float smallError = .5;
  const float largeTimeout = 150;
  const float smallTimeout = 35;

  uint32_t largeTimeoutStart = 0;
  uint32_t smallTimeoutStart = 0;

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() { moveToPoint(x, y, linearPid, headingPid, timeout, maxSpeed); });
  }


  float startPos{0};
  double startHeading = headingTarget;

  state = DriveState::MOVING;
  uint32_t startTime = pros::millis();

  while (true) {
    Point pose = getPose();

    double distance = hypot(pose.x - x, pose.y - y);
    double heading = pose.theta;
    double headingError = constrain180((atan2(y - pose.y, x - pose.x) - radiansToDegrees(heading)) * degreesToRadians(180));
    double linearError = distance * cos(degreesToRadians(headingError));

    if (std::fabs(distance) < 11.5) {
      headingError = 0;
    }

    float linearOutput = (std::fabs(headingError) >= 90) ? linearPid.update(-linearError) : linearPid.update(linearError);

    headingError = constrain90(headingError);

    if (linearOutput > maxSpeed) {
      linearOutput = maxSpeed;
    }
    else if (linearOutput < -maxSpeed) {
      linearOutput = -maxSpeed;
    }
    
    arcade(linearOutput, headingPid.update(headingError));

    pros::delay(10);
  }
}



void Chassis::moveToPose(Point target, PID linearPid, PID headingPid, PID turningPid, int timeout, float maxSpeed, bool async) {
  int startTime = pros::millis();

  moveToPoint(target.x, target.y, linearPid, headingPid, timeout, maxSpeed, async);
  turn(target.theta, turningPid, timeout - (pros::millis() - startTime), maxSpeed);
}