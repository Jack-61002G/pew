
#include "lib/lift.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include "autons.h"
#include "robodash/api.h"
#include <string>



// ================================= Views ================================= //

// Create robodash selector
rd::Selector selector({
    {"RedLeft", &redLeft},
    {"RedRight", &redRight},
    {"redRush", &redRush},
});

// Create robodash console
rd::Console console;

// ========================= Competition Functions ========================= //


void initialize() {
  imu.reset(true);
  track.reset();
  pros::delay(2000);

  chassis.startTask();
  lift.startTask();

}



void disabled() {}



void competition_initialize() {}



void autonomous() {
  selector.run_auton();
}



void opcontrol() {

  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  chassis.moveToPoint(36, 24, linear, movetoturn, 2000);
  chassis.moveToPoint(0, 0, linear, movetoturn, 2000);
  //chassis.turn(0, turning, 1000);
  //chassis.moveToPoint(0, 36, linear, turning, 10000);

    while (true) {
    
    console.clear();
    console.focus();
    std::string str = std::to_string(chassis.getPose().x) + " " + std::to_string(chassis.getPose().y) + " " + std::to_string(chassis.getPose().theta) + "\n";
    console.println(str);

    chassis.arcadeMod(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), 2, 114, 110);
    intake.move((controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) ? -127 : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) ? 127 : 0 );
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {pisstake.toggle();}
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {claw.toggle();}
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {wrist.toggle();}
    armMotors.move((controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) ? -127 : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) ? 127 : 0 );

    pros::delay(15);
  }}
