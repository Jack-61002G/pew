#include "lib/chassis.h"
#include "lib/pd.h"
#include "pros/rtos.hpp"

using namespace lib;

double angleWrap(double angle) {
  while (angle > 360) {
    angle -= 360;
  }
  while (angle < -360) {
    angle += 360;
  }
  return angle;
}

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

void Chassis::pdTurn(double target, int maxSpeed, double timeout,
                     PDconstants constants, bool async) {
  PD pd(constants.getConstants());
  double speed;
  double error = angleWrap(target - imu->get_rotation());


  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task(
        [&]() { pdTurn(target, maxSpeed, timeout, constants, false); });
  }

  int start = pros::millis();
  state = DriveState::MOVING;
  correctHeading = false;
  while (pros::millis() - start < timeout ||
         error < constants.getConstants()[2] &&
             motors->getDiffyVel()[0] < constants.getConstants()[3]) {

    error = angleWrap(target - imu->get_rotation());

    speed = pd.calculate(error);
    if (speed > maxSpeed) {
      speed = maxSpeed;
    } else if (speed < -maxSpeed) {
      speed = -maxSpeed;
    }
    motors->spinDiffy(speed, -speed);
  }
  motors->spinDiffy(0, 0);
  state = DriveState::IDLE;
}

void Chassis::headingTask(PDconstants constants) {

  pros::Task task([&]() {
    PD pd(constants.getConstants());
    double speed;
    double error = angleWrap(headingTarget - imu->get_rotation());

    while (correctHeading) {
      error = angleWrap(headingTarget - imu->get_rotation());

      speed = pd.calculate(error);

      motors->spinDiffy(motors->getDiffyVel()[0] + speed,
                        motors->getDiffyVel()[1] - speed);
      pros::delay(20);
    }
    pros::delay(20);
  });
}