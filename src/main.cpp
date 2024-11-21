
#include "autons.h"
#include "lib/lift.hpp"
#include "lib/trajectory.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robodash/api.h"
#include "robotconfig.h"
#include <string>

// ================================= Views ================================= //

// Create robodash selector
rd::Selector selector({{"Skills", skills},
                       {"Red Ring Side", redRingSide},
                       {"Red Rush", redRush},
                       {"Blue Ring Side", blueRingSide},
                       {"Blue Rush", blueRush}});

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
  intake.startTask();
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

  selector.run_auton();
}

void opcontrol() {

  //chassis.team = 1;
  //lights.team = 1;

  lights.startTimer();
  float liftTarget = -1;

  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  while (true) {

    chassis.arcadeMod(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
                      controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X),
                      2, 114, 110);
    lib::IntakeState newState =
        (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
            ? lib::IntakeState::In
        : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
            ? lib::IntakeState::Out
            : lib::IntakeState::Idle;

    if (intake.getState() != newState) {
      intake.setState(newState);
    }
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      intake.sort_override = true;
    } else {
      intake.sort_override = false;
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      doinker.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      clamp.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
      lift.itterateState(1);
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
      pisstake.toggle();
    }

    pros::delay(15);
  }
}
