
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
  lights.startTask();
  lights.team = 1;
}

void disabled() { clamp.retract(); lights.stopTimer(); }

void competition_initialize() {}



void autonomous() {
  console.focus();
  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  lights.stopTimer();

  /*
  //skills auton
  //starts at red goal
  //place ring on red goal
  chassis.moveToPoint(0, 0, linear, heading); //picking up clamp
  //clamp
  chassis.swing(90,idk, 0, swing); // turn to face ring
  chassis.moveToPoint(0, 0, linear, heading); //pick up ring
  chassis.swing(180, idk, 0, swing); //turn to face ring below robot
  chassis.moveToPoint(0, 0, linear, heading); //pick up ring
  chassis.swing(270, idk, 0, swing); //turn to face the two rings
  chassis.moveToPoint(0, 0, linear, heading); //pick up rings
  chassis.swing(idk, idk, 0, swing); //turn to face last ring
  chassis.moveToPoint(0, 0, linear, heading); //pick up ring
  chassis.swing(45, idk, 0, swing); //turn to point goal into goal
  chassis.moveToPoint(0, 0, linear, heading); //put into goal
  //clamp
  chassis.moveToPoint(0, 0, linear, heading); //slightly pull away from goal
  chassis.swing(idk, idk, 0, swing); //turn to face ring above bottom goal
  chassis.moveToPoint(0, 0, linear, heading); //move claw into position to grab ring
  //At the bottom tall goal
  //claw
  chassis.swing(180,idk, 0, swing); //line up claw to goal
  //move arm up
  //claw
  chassis.swing(idk, idk, 0, swing); //turn to the bottom left of the middle
  chassis.moveToPoint(0, 0, linear, heading); //move to the bottom left of the middle
  chassis.swing(45, idk, 0, swing); //turn to the center
  chassis.moveToPoint(0, 0, linear, heading); //move to the center ring
  //In the middle
  chassis.swing(idk, idk, 0, swing); //turn to face away from the mobile goal
  chassis.moveToPoint(0, 0, linear, heading); //move backwords to the mobile goal
  //clamp
  chassis.swing(idk, idk, 0, swing); //turn to face the closes ring to the middle
  chassis.moveToPoint(0, 0, linear, heading); //move to the ring
  //In the top left quadrent
  chassis.swing(0, idk, 0, swing); //turn to face the ring above
  chassis.moveToPoint(0, 0, linear, heading); //move to the ring
  chassis.swing(270, idk, 0, swing); //turn to face the rings to the left
  chassis.moveToPoint(0, 0, linear, heading); //move to the rings
  chassis.swing(idk, idk, 0, swing); //turn to the last ring
  chassis.moveToPoint(0, 0, linear, heading); //move to the last ring
  chassis.swing(idk, idk, 0, swing); //turn to face away from the goal
  chassis.moveToPoint(0, 0, linear, heading); //move backwords into goal
  //clamp
  chassis.moveToPoint(0, 0, linear, heading); //move away from goal slightly
  chassis.swing(idk, idk, 0, swing); //turn to face the ring below the goal for claw
  chassis.moveToPoint(0, 0, linear, heading); //move to the ring
  //At the bottom tall goal
  //claw
  chassis.swing(180, idk, 0, swing); //turn to face the tall goal
  //arm up
  //claw
*/

  selector.run_auton();
}



void opcontrol() {

  lift.stopTask();
  lights.startTimer();
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
      doinker.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      sorter.toggle();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      clamp.toggle();
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
