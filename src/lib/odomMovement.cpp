#include "lib/chassis.h"
#include "point.hpp"
#include "util.h"
#include <cmath>
#include <cstdlib>

using namespace lib;

void Chassis::moveToPoint(float x, float y, PID linearPid, PID headingPid,
                          int timeout, float maxSpeed, bool async) {
  const float largeError = 1;
  const float smallError = .5;
  const float largeTimeout = 150;
  const float smallTimeout = 35;

  uint32_t largeTimeoutStart = 0;
  uint32_t smallTimeoutStart = 0;

  Point targetPoint = Point(x, y);

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task(
        [&]() { moveToPoint(x, y, linearPid, headingPid, timeout, maxSpeed); });
  }

  float prevError = 0;

  while (true) {
    Point currentPosition = getPose(); // Get position from odometry
    

    // update error
    float deltaX = targetPoint.x - currentPosition.x;
    float deltaY = targetPoint.y - currentPosition.y;
    float targetTheta =
        fmod(radiansToDegrees(M_PI_2 - atan2(deltaY, deltaX)), 360);
    float hypot = std::hypot(deltaX, deltaY);
    float diffTheta1 = angleError(currentPosition.theta, targetTheta);
    float diffTheta2 = angleError(currentPosition.theta, targetTheta + 180);
    float angularError = (std::fabs(diffTheta1) < std::fabs(diffTheta2))
                             ? diffTheta1
                             : diffTheta2;
    float lateralError = hypot * fabs(cos(degreesToRadians(std::fabs(diffTheta1))));

    std::cout << "angular error: " << angularError
              << " lateral error: " << lateralError << std::endl;

    float linearoutput = (angularError <= 90) ? linearPid.update(lateralError) : linearPid.update(-lateralError);

    if (linearoutput > maxSpeed) {
      linearoutput = maxSpeed;
    } else if (linearoutput < -maxSpeed) {
      linearoutput = -maxSpeed;
    }

    if (lateralError <= 11.5) {
      angularError = 0;

      if (prevError < lateralError) {return;}
    }

    prevError = lateralError;
    
    arcade(linearoutput, headingPid.update(angularError));
  }
}

void Chassis::moveToPose(Point target, PID linearPid, PID headingPid,
                         PID turningPid, int timeout, float maxSpeed,
                         bool async) {
  int startTime = pros::millis();

  moveToPoint(target.x, target.y, linearPid, headingPid, timeout, maxSpeed,
              async);
  turn(target.theta, turningPid, timeout - (pros::millis() - startTime),
       maxSpeed);
}