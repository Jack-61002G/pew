#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robotconfig.h"


void initialize() {
  pros::lcd::initialize();
  imu.reset(true);
  imu.tare();

}



void disabled() {}



void competition_initialize() {}



void autonomous() {}



void opcontrol() {
  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  //chassis.move(-18, linear, heading, 1000);
  //chassis.move(18, linear, heading, 1000);

  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  while (true) {
    chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
    intake.move( (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) ? -127 : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) ? 127 : 0 );
    pros::delay(10);
  }
}