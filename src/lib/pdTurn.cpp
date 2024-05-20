#include "lib/chassis.h"
#include "lib/pd.h"
#include "pros/rtos.hpp"

using namespace lib;

#define degreesToRadians(angleDegrees) ((angleDegrees)*M_PI / 180.0)

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

void Chassis::pdTurn(double target, int maxSpeed, double timeout, bool async) {
  PD pd(angularConstants->getConstants());
  double speed;
  double error = angleWrap(target - imu->get_rotation());


  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task(
        [&]() { pdTurn(target, maxSpeed, timeout, false); });
  }

  int start = pros::millis();
  state = DriveState::MOVING;
  while (pros::millis() - start < timeout ||
         error < angularConstants->getConstants()[2] &&
             leftMotors->get_actual_velocity(0) < angularConstants->getConstants()[3]) {

    error = angleWrap(target - imu->get_rotation());

    speed = pd.calculate(degreesToRadians(error));
    if (speed > maxSpeed) {
      speed = maxSpeed;
    } else if (speed < -maxSpeed) {
      speed = -maxSpeed;
    }
    leftMotors->move(speed);
    rightMotors->move(-speed);
  }
  leftMotors->brake();
  rightMotors->brake();
  state = DriveState::IDLE;
}