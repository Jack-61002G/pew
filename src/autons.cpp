#include "lib///lift.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include "autons.h"

void exit_condition_defaults() {
  turning.exit_condition_set(150, 1, 300, 3, 500, 500);
  linear.exit_condition_set(150, .25, 300, .75, 500, 500);
  swing.exit_condition_set(75, 2, 150, 6, 500, 500);
}



void skills() {

    intake.move(127);
    pros::delay(750);
    intake.move(0);

    chassis.turn(130, turning);
    chassis.move(-24, linear, heading);
    clamp.extend();

    chassis.turn(-10, turning);
    intake.move(127);
    chassis.move(24, linear, heading);
    chassis.turn(-90, turning);
    chassis.move(24, linear, heading);

    chassis.turn(180, turning);
    chassis.move(36, linear, heading);
    pros::delay(300);
    chassis.move(-24, linear, heading);
    chassis.turn(-135, turning);
    chassis.move(12, linear, heading);
    chassis.turn(30, turning);
    chassis.move(-20, linear, heading);

    clamp.retract();


}


void redLeft() {
    pros::Task task([&]() {
        pros::delay(800);
        clamp.extend();
    });
    chassis.moveToPoint(0, -26, linear, heading, true, 80);

    intake.move(127);

    chassis.moveToPoint(10, -47, linear, turning);
    chassis.swing(90, true, 0, swing);

    chassis.move(18, linear, heading);
    pros::delay(300);
    
    chassis.moveToPoint(24, -24, linear, heading);

    clamp.retract();
    chassis.moveToPoint(-36, -4, linear, heading, false, 90);
    
    chassis.moveToPoint(-24, 12, linear, heading, true);
    chassis.turn(180, turning);

    pros::delay(500);
    intake.move(0);

    chassis.move(40, linear, heading);
}