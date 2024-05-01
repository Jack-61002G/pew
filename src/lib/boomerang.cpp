#include "lib/chassis.h"
#include "pd.h"

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
    //if the robot is no longer getting closer to the target we should exit
    if (pros::millis() - now > 100 && (start.distanceTo(target)) >= previousD) {
      break;
    }


    Point carrot(target.x - d * cos(theta) * dLead,
                 target.y - d * sin(theta) * dLead);
    linearError = odom->getPose().distanceTo(carrot);
    linearPower = linearPD.update(linearError);
    angularError = odom->getPose().angleError(target);
    angularPower = angularPD.update(angularError);

    double previousD = start.distanceTo(target);

    float leftPower = linearPower + angularPower;
    float rightPower = linearPower - angularPower;

    motors->spinDiffy(leftPower, rightPower);
  }
    motors->spinDiffy(0, 0);
    state = DriveState::IDLE;
}
