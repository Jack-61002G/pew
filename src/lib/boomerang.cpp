#include "lib/chassis.h"
#include "pd.h"
#include <cmath>

using namespace lib;

#define degreesToRadians(angleDegrees) ((angleDegrees)*M_PI / 180.0)

void Chassis::boomerang(double x, double y, double theta, int timeout,
                        double dLead, double gLead, bool async) {

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
    pros::Task task(
        [&]() { boomerang(x, y, theta, timeout, dLead, gLead, false); });
  }

  int now = pros::millis();
  state = DriveState::MOVING;
  correctHeading = false;

  while (true) {

    if (pros::millis() - now > timeout) {
      break;
    }
    // if the robot is no longer getting closer to the target we should exit
    if ((odom->getPose().distanceTo(target)) < 5 &&
        (odom->getPose().distanceTo(target)) >= previousD) {
      break;
    }

    Point carrot(target.x - d * cos(theta) * dLead,
                 target.y - d * sin(theta) * dLead);

    Point garrot(startCarrot + (carrot - startCarrot) * (1 - gLead));

    linearError = odom->getPose().distanceTo(garrot);
    linearPower = linearPD.update(linearError);
    angularError = odom->getPose().angleError(target);
    angularPower = angularPD.update(angularError);

    double previousD = odom->getPose().distanceTo(target);

    float leftPower = linearPower + angularPower;
    float rightPower = linearPower - angularPower;

    motors->spinDiffy(leftPower, rightPower);
  }
  motors->spinDiffy(0, 0);
  state = DriveState::IDLE;
}
