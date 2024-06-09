#include "main.h"
#include "robotconfig.h"
#include "lib/chassis.h"
#include "lib/lift.hpp"
#include "lib/velControl.h"
#include "pros/motor_group.hpp"
#include "pros/rtos.hpp"



void initialize() {
  pros::lcd::initialize();
}



void disabled() {}



void competition_initialize() {}



void autonomous() {}



void opcontrol() {
  while (true) {
    lib::Chassis.arcade(controller.get_analog(ANALOG_LEFT_Y), controller.get_analog(ANALOG_RIGHT_X), {1, 1});
    pros::delay(10);
  }
}