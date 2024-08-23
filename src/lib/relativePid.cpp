#include "lib/chassis.h"
#include <cstdint>


using namespace lib;



void Chassis::move(float target, PID linearPid, PID headingPid, int timeout, float maxSpeed, bool async) {

  const float largeError = 1.2;
  const float smallError = .7;
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


  float startPos = track->getDistance();
  double startHeading = headingTarget;
  uint32_t startTime = pros::millis();


  state = DriveState::MOVING;

  while (true) {

    double distance = track->getDistance();
    double heading = imu->get_rotation();


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

    float linearOutput = linearPid.update(linearError);
    if (linearOutput > maxSpeed) {linearOutput = maxSpeed;}
    if (linearOutput < -maxSpeed) {linearOutput = -maxSpeed;}

    arcade(linearOutput, headingPid.update(headingError));

    pros::delay(10);
  }


}



void Chassis::turn(double target, PID turningPid, int timeout, float maxSpeed, bool async) {

  const float largeError = 3;
  const float smallError = 1;
  const float largeTimeout = 100;
  const float smallTimeout = 35;

  uint32_t largeTimeoutStart = 0;
  uint32_t smallTimeoutStart = 0;


  headingTarget = target;


  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() { turn(target, turningPid, timeout, maxSpeed); });
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
      turningPid.reset();
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
        turningPid.reset();
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
          turningPid.reset();
          return;
        }
      } else {
        smallTimeoutStart = 0;
    }
    
    float output = turningPid.update(headingError);
    output = (output > 0) ? fmin(output, maxSpeed) : fmax(output, -maxSpeed);

    leftMotors->move(output);
    rightMotors->move(-output);

    pros::delay(10);
  }
}



void Chassis::swing(double target, bool side, float multiplier, PID turningPid, int timeout, float maxSpeed, bool async) {

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
    pros::Task task([&]() { swing(target, side, multiplier, turningPid, timeout, maxSpeed); });
  }

  uint32_t startTime = pros::millis();

  double startHeading = imu->get_rotation();

  while (target - startHeading > 360) {
    target -= 360;
  } while (target - startHeading < 0) {
    target += 360;
  }

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
      turningPid.reset();
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
        turningPid.reset();
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
          turningPid.reset();
          headingTarget = target;
          return;
        }
      } else {
        smallTimeoutStart = 0;
    }
    
    float output = fmin(turningPid.update(headingError), maxSpeed);

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


void Chassis::waitUntilFinished() {
  while (state == DriveState::MOVING) {
    pros::delay(20);
  }
}