#include "lib/chassis.h"

using namespace lib;

#define degreesToRadians(angleDegrees) ((angleDegrees)*M_PI / 180.0)

void Chassis::pdMove(double target, int maxSpeed, double timeout, bool async) {
  PD pd(linearConstants->getConstants());
  PD pdHeading(headingConstants->getConstants());
  double speed;

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() { pdMove(target, maxSpeed, timeout, false); });
  }

  double error =
      target - (motors->getDiffyPos()[0] + motors->getDiffyPos()[1]) / 2;

  const double targetHeading = odom->getPose().theta;
  double angleError = angleWrap(targetHeading - imu->get_rotation());

  int start = pros::millis();
  state = DriveState::MOVING;

  while (pros::millis() - start < timeout ||
         error < linearConstants->getConstants()[2] &&
             motors->getDiffyVel()[0] < linearConstants->getConstants()[3]) {

    error = target - (motors->getDiffyPos()[0] + motors->getDiffyPos()[1]) / 2;

    speed = pd.calculate(error);
    if (speed > maxSpeed) {
      speed = maxSpeed;
    } else if (speed < -maxSpeed) {
      speed = -maxSpeed;
    }

    // heading correction
    angleError = angleWrap(target - imu->get_rotation());
    double headingSpeed = pdHeading.calculate(degreesToRadians(angleError));

    motors->spinDiffy(speed + headingSpeed, speed - headingSpeed);
  }
  motors->spinDiffy(0, 0);
  state = DriveState::IDLE;
}