#include "lib/chassis.h"
#include "pd.h"
#include <cmath>

using namespace lib;

#define degreesToRadians(angleDegrees) ((angleDegrees)*M_PI / 180.0)

void Chassis::boomerang(double x, double y, double theta, int timeout,
                        double dLead, double gLead, bool async,
                        double exitRange) {

  PD linearPD(linearConstants->getConstants());
  PD angularPD(angularConstants->getConstants());

  float linearError;
  float linearPower;
  float angularError;
  float angularPower;
  // calculate target pose in standard form
  Point target(x, y, M_PI_2 - degreesToRadians(theta));

  // capture starting pose
  Point start = odom->getPose();

  double d = start.distanceTo(target);

  double previousD = d;

  Point startCarrot(target.x - d * cos(theta) * dLead,
                    target.y - d * sin(theta) * dLead);

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() {
      boomerang(x, y, theta, timeout, dLead, gLead, false, exitRange);
    });
  }

  int now = pros::millis();
  state = DriveState::MOVING;

  while (true) {

    if (pros::millis() - now > timeout) {
      break;
    }
    // semicircle exit condition
    if ((odom->getPose().y - target.y) * -cos(target.theta) <=
        sin(target.theta) * (odom->getPose().x - target.x) + exitRange) {
      break;
    }

    Point carrot(target.x - d * cos(theta) * dLead,
                 target.y - d * sin(theta) * dLead);

    Point garrot(startCarrot + (carrot - startCarrot) * (1 - gLead));

    linearError = (odom->getPose().distanceTo(garrot) > 2)
                      ? odom->getPose().distanceTo(garrot)
                      : odom->getPose().distanceTo(carrot);
    linearPower = linearPD.update(linearError);
    // if close to garrot, use carrot as heading target, if close to carrot, use
    // target as heading target
    angularError = (odom->getPose().distanceTo(garrot) > 2)
                       ? odom->getPose(true).angleTo(garrot)
                   : (odom->getPose().distanceTo(carrot) > 2)
                       ? odom->getPose(true).angleTo(carrot)
                       : odom->getPose(true).angleError(target);
    angularPower = angularPD.update(angularError);

    double previousD = odom->getPose().distanceTo(target);

    float leftPower = linearPower + angularPower;
    float rightPower = linearPower - angularPower;

    leftMotors->move(leftPower);
    rightMotors->move(rightPower);
  }
  leftMotors->brake();
  rightMotors->brake();
  state = DriveState::IDLE;
}

void Chassis::boomerang(std::vector<Point> waypoints, int timeout, double dLead,
                        double gLead, bool async, double exitRange,
                        double endExitRange) {
  // call boomerang with exitRange except for the last element in the vector,
  // then call the last one with endExitRange
  for (int i = 0; i < waypoints.size() - 2; i++) {
    boomerang(waypoints[i].x, waypoints[i].y, waypoints[i].theta, timeout,
              dLead, gLead, async, exitRange);
  }
  boomerang(waypoints[waypoints.size() - 1].x,
            waypoints[waypoints.size() - 1].y,
            waypoints[waypoints.size() - 1].theta, timeout, dLead, gLead, async,
            endExitRange);
}