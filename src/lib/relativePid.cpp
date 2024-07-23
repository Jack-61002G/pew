#include "lib/chassis.h"
#include <cstdint>


using namespace lib;

float constrain180(float input) {  
  while (input > 180) {
    input -= 360;
  } while (input < -180) {
    input += 360;
  }
    return input;
  }


void Chassis::move(float target, PID linearPid, PID headingPid, int timeout, float maxSpeed, bool async) {

  const float largeError = 1;
  const float smallError = .5;
  const float largeTimeout = 150;
  const float smallTimeout = 35;

  uint32_t largeTimeoutStart = 0;
  uint32_t smallTimeoutStart = 0;

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() { move(target, linearPid, headingPid, timeout, maxSpeed); });
  }


  float startPos{0};
  double startHeading = headingTarget;

  state = DriveState::MOVING;
  uint32_t startTime = pros::millis();

  while (true) {

    double distance = track->getDistance();
    double heading = imu->get_rotation();


    if (startPos == 0) {
      startPos = distance;
    }


    double linearError = startPos + target - distance;
    double headingError = startHeading - heading;


    //ez template style large error/small error exits
    if (pros::millis() - startTime > timeout) {
      leftMotors->move(0);
      rightMotors->move(0);
      leftMotors->brake();
      rightMotors->brake();
      state = DriveState::IDLE;
      linearPid.reset();
      headingPid.reset();
      return;
    }

    if(std::abs(linearError) < largeError) {
      if(largeTimeoutStart == 0) {
        largeTimeoutStart = pros::millis();
      } else if(pros::millis() - largeTimeoutStart > largeTimeout) {
        leftMotors->move(0);
        rightMotors->move(0);
        leftMotors->brake();
        rightMotors->brake();
        state = DriveState::IDLE;
        linearPid.reset();
        headingPid.reset();
        return;
      }
    } else {
      largeTimeoutStart = 0;
    }

    if(std::abs(linearError) < smallError) {
      if(smallTimeoutStart == 0) {
        smallTimeoutStart = pros::millis();
      } else if(pros::millis() - smallTimeoutStart > smallTimeout) {
        leftMotors->move(0);
        rightMotors->move(0);
        leftMotors->brake();
        rightMotors->brake();
        state = DriveState::IDLE;
        linearPid.reset();
        headingPid.reset();
        return;
      }
    } else {
      smallTimeoutStart = 0;
    }

    arcade(linearPid.update(linearError), headingPid.update(headingError));

    pros::delay(10);
  }


}





void Chassis::turn(double target, PID headingPid, int timeout, float maxSpeed, bool async) {

  const float largeError = 3;
  const float smallError = 1;
  const float largeTimeout = 100;
  const float smallTimeout = 35;

  uint32_t largeTimeoutStart = 0;
  uint32_t smallTimeoutStart = 0;

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() { turn(target, headingPid, timeout, maxSpeed); });
  }


  uint32_t startTime = pros::millis();


  state = DriveState::MOVING;


  while (true) {

    double headingError = target - constrain180(imu->get_rotation());

    //ez template style large error/small error exits
    if (pros::millis() - startTime > timeout) {
      leftMotors->move(0);
      rightMotors->move(0);
      leftMotors->brake();
      rightMotors->brake();
      state = DriveState::IDLE;
      headingPid.reset();
      headingTarget = target;
      return;
    }
    if(std::abs(headingError) < largeError) {

      if(largeTimeoutStart == 0) {
        largeTimeoutStart = pros::millis();
      } else if(pros::millis() - largeTimeoutStart > largeTimeout) {
        leftMotors->move(0);
        rightMotors->move(0);
        leftMotors->brake();
        rightMotors->brake();
        state = DriveState::IDLE;
        headingPid.reset();
        headingTarget = target;
        return;
      }
    } else {
      largeTimeoutStart = 0;
    }
    if(std::abs(headingError) < smallError) {
        
        if(smallTimeoutStart == 0) {
          smallTimeoutStart = pros::millis();
        } else if(pros::millis() - smallTimeoutStart > smallTimeout) {
          leftMotors->move(0);
          rightMotors->move(0);
          leftMotors->brake();
          rightMotors->brake();
          state = DriveState::IDLE;
          headingPid.reset();
          headingTarget = target;
          return;
        }
      } else {
        smallTimeoutStart = 0;
    }
    
    float output = headingPid.update(headingError);
    output = (output > 0) ? fmin(output, maxSpeed) : fmax(output, -maxSpeed);

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
      rightMotors->move(output);
    } else {
      leftMotors->move(output);
      rightMotors->move(output * multiplier);
    }

    pros::delay(10);
  }
}