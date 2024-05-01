#include "lib/chassis.h"

using namespace lib;

void Chassis::pdMove(double target, int maxSpeed, double timeout, bool async) {
  PD pd(linearConstants->getConstants());
  double speed;
  double error = target - (motors->getDiffyPos()[0] + motors->getDiffyPos()[1]) / 2;

  if (async) {
    while(this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task(
        [&]() { pdMove(target, maxSpeed, timeout,false); });
  }

  int start = pros::millis();
  state = DriveState::MOVING;
  correctHeading = true;
  while (pros::millis() - start < timeout || error < linearConstants->getConstants()[2] && motors->getDiffyVel()[0] < linearConstants->getConstants()[3]){

    error = target - (motors->getDiffyPos()[0] + motors->getDiffyPos()[1]) / 2;

    speed = pd.calculate(error);
    if (speed > maxSpeed) {
      speed = maxSpeed;
    } else if (speed < -maxSpeed) {
      speed = -maxSpeed;
    }
    
    motors->spinDiffy(speed, speed);
  }
  motors->spinDiffy(0, 0);
  state = DriveState::IDLE;
}