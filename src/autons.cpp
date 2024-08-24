#include "lib/lift.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include "autons.h"



void redLeft() {
    pros::Task task([&]() {
        pros::delay(640);
        clamp.extend();
    });

    chassis.move(-28.25, linear, heading, 1200, 85);

    pros::delay(300);

    chassis.turn(99, turning, 1000);
    intake.move(-127);
    chassis.move(22.5, linear, heading, 1000, 90);

    pros::delay(100);

    chassis.move(-14, linear, heading, 1000);

    chassis.turn(140, turning, 1000);
    chassis.move(19, linear, heading, 1000);
    pros::delay(350);
    chassis.move(-19, linear, heading, 1000);

    chassis.turn(90, turning, 1000);
    intake.move(0);
    chassis.move(24, linear, heading, 1000);

    chassis.turn(-175, turning, 1000);
    intake.move(-127);
    chassis.move(19, linear, heading, 1000);
    pros::delay(350);
    chassis.turn(-180, turning, 1000);
    chassis.move(-40, linear, heading, 1000);
}

void redRight() {
    
    pros::Task task([&]() {
        pros::delay(640);
        clamp.extend();
    });

    chassis.move(-28.25, linear, heading, 1200, 85);

    pros::delay(300);

    chassis.turn(-99, turning, 1000);
    intake.move(-127);
    chassis.move(22.5, linear, heading, 1000, 90);

    pros::delay(750);
    chassis.turn(-15.5, turning, 500);

    pros::delay(150);
    intake.move(0);
    lift.setAngle(18.15);

    chassis.move(27, linear, heading, 1000, 90);
    chassis.move(9.53, linear, heading, 1000, 55);

    claw.extend();
    pros::delay(200);
    lift.setAngle(60);

    chassis.move(-9.55, linear, heading, 500);
    chassis.turn(89.25, turning, 800);

    pisstake.extend();
    intake.move(-127);
    chassis.move(45, linear, heading, 1000, 100);

    pros::delay(200);
    pisstake.retract();
    pros::delay(200);
    chassis.move(-6.5, linear, heading, 1000);

    chassis.turn(42.72, turning, 500);

    pros::delay(1200);
    intake.move(0);

    chassis.move(9.712, linear, heading, 800, 80);

    pros::delay(250);
    lift.setAngle(30);
    pros::delay(450);
    claw.retract();

    chassis.move(-36, linear, heading, 1000);
    lift.setAngle(70);
    chassis.turn(135, turning, 1000);
    clamp.retract();
    chassis.move(22, linear, heading, 1000);
}

void redRush() {

    // TODO set starting pos to match angle
    
    // Rush mogo on auton line
    chassis.move(-60, linear, heading, 1000);
    clamp.extend();
    intake.move(-127);

    chassis.turn(15, turning, 500);
    chassis.move(12, linear, heading, 1000);

    pros::delay(500);

    clamp.retract();
    intake.move(0);
    chassis.turn(-90, turning, 800);
    chassis.move(-24, linear, heading, 1000);

    clamp.extend();


    // turn to other mogo
    // grab mogo

    // yoink bottom ring from stack

    // turn to corner
    // move to corner stack

    // pull top ring off with arm
    // intake ring
}

void blueLeft() {
    return;
}

void blueRight() {
    return;
}

void blueRush() {
    return;
}