#include "lib/chassis.h"
#include <cmath>

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

    double distance = -sqrt((getPose().x - x) + (getPose().y - y));
    double heading = imu->get_rotation();
    double headingError = atan2(x - getPose().x, y - getPose().y) - heading;


    //ez template style large error/small error exits
    if (pros::millis() - startTime > timeout) {
      leftMotors->move(0);
      rightMotors->move(0);
      leftMotors->brake();
      rightMotors->brake();
      state = DriveState::IDLE;
      linearPid.reset();
      headingPid.reset();
      return;
    }

    if(std::abs(distance) < largeError) {
      if(largeTimeoutStart == 0) {
        largeTimeoutStart = pros::millis();
      } else if(pros::millis() - largeTimeoutStart > largeTimeout) {
        leftMotors->move(0);
        rightMotors->move(0);
        leftMotors->brake();
        rightMotors->brake();
        state = DriveState::IDLE;
        linearPid.reset();
        headingPid.reset();
        return;
      }
    } else {
      largeTimeoutStart = 0;
    }

    if(std::abs(distance) < smallError) {
      if(smallTimeoutStart == 0) {
        smallTimeoutStart = pros::millis();
      } else if(pros::millis() - smallTimeoutStart > smallTimeout) {
        leftMotors->move(0);
        rightMotors->move(0);
        leftMotors->brake();
        rightMotors->brake();
        state = DriveState::IDLE;
        linearPid.reset();
        headingPid.reset();
        return;
      }
    } else {
      smallTimeoutStart = 0;
    }

    arcade(linearPid.update(distance), headingPid.update(headingError));

    pros::delay(10);
  }
}



void Chassis::moveToPose(Point target, PID linearPid, PID headingPid, PID turningPid, int timeout, float maxSpeed, bool async) {
  int startTime = pros::millis();

  moveToPoint(target.x, target.y, linearPid, headingPid, timeout, maxSpeed, async);
  turn(target.theta, turningPid, timeout - (pros::millis() - startTime), maxSpeed);
}