#include "lib/lift.hpp"
#include "liblvgl/llemu.hpp"
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

  moving = true;
  setState(LiftState::MOVE);
}

void Lift::loop() {

  uint32_t now = pros::millis();
  while (true) {
    switch (getState()) {

    case LiftState::IDLE:
      break;

    case LiftState::MOVE:
      uint32_t start = pros::millis();
      double integral = 0;
      double prevError = 0;
      double integralMax = 70; // Adjust this value as needed

      while (true) {

        double error = target - motors.get_position();
        double derivative = error - prevError;

        integral += error;

        // Anti-windup: Limit the integral term
        if (integral > integralMax) {
          integral = integralMax;
        } else if (integral < -integralMax) {
          integral = -integralMax;
        }

        double output = constants.kP * error + constants.kI * integral +
                        constants.kD * derivative;

        motors.move(output);

        prevError = error;

        if (error < 1 && motors.get_actual_velocity() < 5 &&
            pros::millis() - start > 1000) {
          moving = false;
        }

        pros::Task::delay_until(&now, 25);
        pros::lcd::set_text(2, std::to_string(motors.get_position()));
        std::cout << motors.get_position() << std::endl;
      }
      break;
    }
    pros::Task::delay_until(&now, 15);
  }
}