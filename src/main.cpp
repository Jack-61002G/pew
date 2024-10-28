
#include "autons.h"
#include "lib/lift.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robodash/api.h"
#include "robotconfig.h"
#include <string>

// ================================= Views ================================= //

// Create robodash selector
rd::Selector selector({{"Skills", skills}});

// ========================= Competition Functions ========================= //

void initialize() {

  exit_condition_defaults();

  imu.reset();

  while (imu.is_calibrating()) {
    pros::delay(10);
  }

  track.reset();
  pros::delay(500);

  chassis.startTask();
  lift.startTask();
  lights.startTask();
}

void disabled() {
  clamp.retract();
  lights.stopTimer();
}

void competition_initialize() {}

void autonomous() {
  console.focus();
  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  lights.stopTimer();

  redLeft();

  //selector.run_auton();
}

void opcontrol() {

  lights.startTimer();
  float liftTarget = -1;

  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  while (true) {

    chassis.arcadeMod(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
                      controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X),
                      2, 114, 110);
    intake.move((controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))   ? 127
                : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) ? -127
                                                                          : 0);
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      sorter.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      doinker.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      clamp.toggle();
      lights.indicator = clamp.is_extended();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
      lift.itterateState(1);
    }

    pros::delay(15);
  }
}
