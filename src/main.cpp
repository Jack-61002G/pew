
#include "autons.h"
#include "lib/lift.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robodash/api.h"
#include "robotconfig.h"
#include <string>

// ================================= Views ================================= //

// Create robodash selector
rd::Selector selector({{"RedLeft", &redLeft},
                       {"RedRight", &redRight},
                       {"redAWP", &redAWP},
                       {"BlueLeft", &blueLeft},
                       {"BlueRight", &blueRight},
                       {"blueAWP", &blueAWP}});



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
}

void disabled() { clamp.retract(); }

void competition_initialize() {}



void autonomous() {
  console.focus();
  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  chassis.swing(90, true, 0, swing);
  chassis.swing(90, true, 0, swing);
  chassis.swing(90, false, 0, swing);
  chassis.swing(0, false, 0, swing);

  //selector.run_auton();
}



void opcontrol() {

  lift.stopTask();
  float liftTarget = -1;

  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  while (true) {

    chassis.arcadeMod(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
                      controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X),
                      2, 114, 110);
    intake.move((controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))   ? -127
                : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) ? 127
                                                                          : 0);
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      pisstake.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      claw.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      clamp.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
      wrist.toggle();
    }
    armMotors.move(
        (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))   ? -100
        : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) ? 100
                                                                  : 0);

    if (liftTarget == -1) {
      liftTarget = armMotors.get_position();
    }
    if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
        !controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      armMotors.move((liftTarget - armMotors.get_position()) * 1.5);
    } else {
      liftTarget = -1;
    }
    std::cout << liftTarget << std::endl;

    pros::delay(15);
  }
}
