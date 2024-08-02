#include "lib/lift.hpp"
#include "pros/abstract_motor.hpp"
#include <cstdint>
#include <iostream>

using namespace lib;

void Lift::waitUntilSettled() {
  while (getState() != LiftState::IDLE) {
    pros::delay(15);
  }
}

void Lift::setAngle(double angle) {

  setState(LiftState::IDLE);

  this->target = angle / gearRatio;

  setState(LiftState::MOVE);
}

void Lift::loop() {

  uint32_t now = pros::millis();
  while (true) {
    switch (getState()) {

    case LiftState::IDLE:
      std::cout << "Lift idle" << std::endl;

      motors->move(0);
      motors->set_brake_mode_all(pros::MotorBrake::coast);
      motors->brake();
      break;

    case LiftState::MOVE:
      std::cout << "Moving lift" << std::endl;
      
      uint32_t start = pros::millis();
      double integral = 0;
      double prevError = 0;
      double integralMax = 10;

      while (true) {

        double error = target - motors->get_position();
        double derivative = error - prevError;

        integral += error;

        // Anti-windup: Limit the integral term
        if (integral > integralMax) {
          integral = integralMax;
        } else if (integral < -integralMax) {
          integral = -integralMax;
        }

                // Check for oscillation
        if (error * prevError < 0 && target == 0 && pros::millis() - start > 250 && std::abs(error) < 7) {
          setState(LiftState::IDLE);
          break;
        }

        double output = constants.kP * error + constants.kI * integral + constants.kD * derivative;

        motors->move(output);

        prevError = error;

        pros::Task::delay_until(&now, 25);
      }
      break;
    }
    pros::Task::delay_until(&now, 15);
  }
}