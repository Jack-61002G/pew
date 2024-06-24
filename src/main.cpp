#include "pros/misc.h"
#include "robotconfig.h"


void initialize() {
  pros::lcd::initialize();
}



void disabled() {}



void competition_initialize() {}



void autonomous() {}



void opcontrol() {
  chassis.move(-12, linear);
  chassis.move(12, linear);

  while (true) {
    chassis.arcade(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), {1, 1});
    intake.move( (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) ? -127 : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) ? 127 : 0 );
    pros::delay(10);
  }
}