#include "lib/chassis.h"
#include "lib/util.h"
#include "pros/llemu.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <cstdint>
#include <vector>



using namespace lib;



void Chassis::move(float target, PID linearPid, PID headingPid, int timeout, float maxSpeed) {
  float startPos;
  double startHeading;

  state = DriveState::MOVING;


  while (true) {

    std::vector<double> lPositions = leftMotors->get_position_all();
    std::vector<double> rPositions = rightMotors->get_position_all();
    std::vector<float> distances;
    for (int i = 0; i < lPositions.size(); i++) {
      distances.push_back(lPositions[i] * (wheel * M_PI) * (rpm / 600.0)); // assumes blue gear cartridges
    }
    for (int i = 0; i< rPositions.size(); i++) {
      distances.push_back(rPositions[i] * (wheel * M_PI) * (rpm / 600.0));
    }


    double heading = imu->get_rotation();


    if (startPos == 0) {
      startPos = avg(distances);
      startHeading = heading;
    }


    double linearError = startPos + target - avg(distances);
    double headingError = startHeading - heading;

    pros::lcd::print(1, "error: %f", linearError);

    if (std::abs(linearError) < 0.5 && std::abs(headingError) < 1.0 && std::abs(leftMotors->get_actual_velocity()) < 5 && std::abs(rightMotors->get_actual_velocity()) < 5) {
      leftMotors->move(0);
      rightMotors->move(0);
      state = DriveState::IDLE;
      linearPid.reset();
      headingPid.reset();
      return;
    }
    
    arcade(fmin(linearPid.update(linearError), maxSpeed), headingPid.update(headingError));
  }
}



void Chassis::turn(double target, PID headingPid, int timeout, float maxSpeed) {
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

    if (std::abs(headingError) < 1.0 && std::abs(leftMotors->get_actual_velocity()) < 5 && std::abs(rightMotors->get_actual_velocity()) < 5) {
      leftMotors->move(0);
      rightMotors->move(0);
      state = DriveState::IDLE;
      headingPid.reset();
      return;
    }
    
    float output = fmin(headingPid.update(headingError), maxSpeed);

    leftMotors->move(output);
    rightMotors->move(-output);
  }
}



void Chassis::swing(double target, bool side, float multiplier, PID headingPid, int timeout, float maxSpeed) {
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

    if (std::abs(headingError) < 1.0 && std::abs(leftMotors->get_actual_velocity()) < 5 && std::abs(rightMotors->get_actual_velocity()) < 5) {
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
  }
}