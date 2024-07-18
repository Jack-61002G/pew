#include "lib/chassis.h"
#include "lib/util.h"
#include "pros/llemu.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <cstdint>
#include <vector>



using namespace lib;



void Chassis::move(float target, PID linearPid, PID headingPid, int timeout, float maxSpeed, bool async) {
  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() { move(target, linearPid, headingPid, timeout, maxSpeed); });
  }


  float startPos;
  double startHeading;

  state = DriveState::MOVING;


  while (true) {

    double distance = track->getDistance();
    double heading = imu->get_rotation();


    if (startPos == 0) {
      startPos = distance;
      startHeading = heading;
    }


    double linearError = startPos + target - distance;
    double headingError = startHeading - heading;


    if (std::abs(linearError) < 0.5 && std::abs(leftMotors->get_actual_velocity()) < 10 && std::abs(rightMotors->get_actual_velocity()) < 10) {
      leftMotors->move(0);
      rightMotors->move(0);
      state = DriveState::IDLE;
      linearPid.reset();
      headingPid.reset();
      return;
    }

    arcade(linearPid.update(linearError), headingPid.update(headingError));

    pros::delay(10);
  }
}



void Chassis::turn(double target, PID headingPid, int timeout, float maxSpeed, bool async) {
  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() { turn(target, headingPid, timeout, maxSpeed); });
  }


  double startHeading = imu->get_rotation();

  while (target - startHeading > 360) {
    target -= 360;
  } while (target - startHeading < 0) {
    target += 360;
  }

  state = DriveState::MOVING;


  while (true) {

    double headingError = target + startHeading - imu->get_rotation();

    pros::lcd::print(1, "error: %f", headingError);

    if (std::abs(headingError) < 2 && std::abs(leftMotors->get_actual_velocity()) < 5 && std::abs(rightMotors->get_actual_velocity()) < 5) {
      leftMotors->move(0);
      rightMotors->move(0);
      state = DriveState::IDLE;
      headingPid.reset();
      return;
    }
    
    float output = fmin(headingPid.update(headingError), maxSpeed);

    leftMotors->move(output);
    rightMotors->move(-output);

    pros::delay(10);
  }
}



void Chassis::swing(double target, bool side, float multiplier, PID headingPid, int timeout, float maxSpeed, bool async) {
  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() { swing(target, side, multiplier, headingPid, timeout, maxSpeed); });
  }


  double startHeading = imu->get_rotation();

  while (target - startHeading > 360) {
    target -= 360;
  } while (target - startHeading < 0) {
    target += 360;
  }

  state = DriveState::MOVING;


  while (true) {

    double headingError = target + startHeading - imu->get_rotation();

    pros::lcd::print(1, "error: %f", headingError);

    if (std::abs(headingError) < 2.0 && std::abs(leftMotors->get_actual_velocity()) < 5 && std::abs(rightMotors->get_actual_velocity()) < 5) {
      leftMotors->move(0);
      rightMotors->move(0);
      state = DriveState::IDLE;
      headingPid.reset();
      return;
    }
    
    float output = fmin(headingPid.update(headingError), maxSpeed);

    if (side) {
      leftMotors->move(output * multiplier);
      rightMotors->move(-output);
    } else {
      leftMotors->move(output);
      rightMotors->move(-output * multiplier);
    }

    pros::delay(10);
  }
}