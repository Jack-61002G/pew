#include "lib/lift.hpp"
#include "pros/abstract_motor.hpp"
#include <cstdint>

using namespace lib;

void Lift::waitUntilSettled() {
  while (moving) {
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
      motors->set_brake_mode_all(pros::MotorBrake::hold);
      motors->brake();
      break;

    case LiftState::MOVE:
      uint32_t start = pros::millis();
      double integral = 0;
      double prevError = 0;
      double integralMax = 10; // Adjust this value as needed

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

        double output = constants.kP * error + constants.kI * integral + constants.kD * derivative;

        motors->move(output);

        prevError = error;

        if (error < 1 && motors->get_actual_velocity() < 5 && pros::millis() - start > 1000) {
          setState(LiftState::IDLE);
        }

        pros::Task::delay_until(&now, 25);
      }
      break;
    }
    pros::Task::delay_until(&now, 15);
  }
}