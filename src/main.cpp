#include "robotconfig.h"


void initialize() {
  pros::lcd::initialize();
}



void disabled() {}



void competition_initialize() {}



void autonomous() {}



void opcontrol() {
  while (true) {
    chassis.arcade(controller.get_analog(ANALOG_LEFT_Y), controller.get_analog(ANALOG_RIGHT_X), {1, 1});
    pros::delay(10);
  }
}