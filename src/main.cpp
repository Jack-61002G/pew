
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include "robodash/api.h"
#include <string>


// ============================= Example autons ============================= //

void best_auton() { std::cout << "Running best auton" << std::endl; }
void simple_auton() { std::cout << "Running simple auton " << std::endl; }
void good_auton() { std::cout << "Running good auton" << std::endl; }

// ================================= Views ================================= //

// Create robodash selector
rd::Selector selector({
    {"Best auton", &best_auton},
    {"Simple auton", &simple_auton},
    {"Good auton", &good_auton},
});

// Create robodash console
rd::Console console;

// ========================= Competition Functions ========================= //


void initialize() {
  //leftDriveLed.set_all(0x00ff00);

  //rightDriveLed.set_all(0xff0000);

  
  imu.reset(true);
  track.reset();
  pros::delay(500);

  chassis.startTask();
  //lift.startTask();

}



void disabled() {}



void competition_initialize() {}



void autonomous() {}



void opcontrol() {

  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  //chassis.swing(90, false, 0.6, turning, 10000, 127);
  //chassis.turn(90, turning, 10000);
  //chassis.turn(-90, turning, 10000);
  // chassis.turn(0, turning, 10000);

  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  console.focus();

  //rightDriveLed.set_all(0xff0000);
  //rightArmLed.set_all(0xff0000);
  //leftArmLed.set_all(0xff0000);
  //chassis.move(12, linear, heading, 1000);
  chassis.moveToPoint(24, 24, linear, turning, 1500);
  chassis.turn(-135, turning, 1500);
  chassis.moveToPoint(0, 0, linear, turning, 1500);

    while (true) {
    
    console.clear();
    std::string str = std::to_string(chassis.getPose().x) + " " + std::to_string(chassis.getPose().y) + " " + std::to_string(chassis.getPose().theta) + "\n";
    console.println(str);

    chassis.arcadeMod(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), 2, 114, 110);
    intake.move((controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) ? -127 : (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) ? 127 : 0 );
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {pisstake.toggle();}
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {claw.toggle();}
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {clamp.toggle();}

    pros::delay(15);
  }}
