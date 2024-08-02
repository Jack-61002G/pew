#include "lib/chassis.h"
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

    double dx = getPose().x - x;
    double dy = getPose().y - y;
    double distance = sqrt(dx * dx + dy * dy);
    double heading = imu->get_rotation();
    double headingError = (atan2(y - getPose().y, x - getPose().x) - (heading * M_PI / 180)) * 180 / M_PI;

    std::cout << "Distance: " << distance << " Heading: " << heading << " Heading Error: " << headingError << std::endl;


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

    arcade(linearPid.update(distance * (1 - fabs(headingError) / 90)), headingPid.update(headingError));

    pros::delay(10);
  }
}



void Chassis::moveToPose(Point target, PID linearPid, PID headingPid, PID turningPid, int timeout, float maxSpeed, bool async) {
  int startTime = pros::millis();

  moveToPoint(target.x, target.y, linearPid, headingPid, timeout, maxSpeed, async);
  turn(target.theta, turningPid, timeout - (pros::millis() - startTime), maxSpeed);
}