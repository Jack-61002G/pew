#include "lib/lift.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include "autons.h"



void redLeft() {
    chassis.move(-26.25, linear, heading, 1000);
    clamp.extend();
    pros::delay(100);

    chassis.turn(110, turning, 1000);
    intake.move(-127);
    chassis.move(22.5, linear, heading, 1000);

    pros::delay(100);

    chassis.move(-3, linear, heading, 1000);

    chassis.turn(180, turning, 1000);
    chassis.move(16, linear, heading, 1000);
    pros::delay(100);
    chassis.move(-11, linear, heading, 1000);

    chassis.turn(123, turning, 1000);
    chassis.swing(180, false, 0.3, turning, 1000, 100);
    pros::delay(200);
    intake.move(0);

    chassis.move(-50, linear, heading, 1000);
    chassis.turn(45, turning, 1000);


    // grab rings from 4 stack

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

    intake.move(0);
    lift.setAngle(18.15);

    chassis.move(30, linear, heading, 1000, 90);
    chassis.move(6.6, linear, heading, 1000, 60);

    claw.extend();
    pros::delay(200);
    lift.setAngle(60);

    chassis.move(-9.55, linear, heading, 500);
    chassis.turn(90, turning, 800);

    pisstake.extend();
    intake.move(-127);
    chassis.move(47, linear, heading, 1000, 100);

    pros::delay(200);
    pisstake.retract();
    pros::delay(100);
    chassis.move(-8.5, linear, heading, 1000);

    pros::delay(1200);

    chassis.turn(43.5, turning, 500);
    intake.move(0);

    chassis.move(10.2, linear, heading, 800, 80);

    pros::delay(250);
    lift.setAngle(33);
    pros::delay(400);
    claw.retract();

    chassis.move(-36, linear, heading, 500);
    lift.setState(lib::LiftState::UP_OUT);
    chassis.turn(135, turning, 800);
    clamp.retract();
    chassis.move(24, linear, heading, 1000);


    //lift.setState()

    // Turn to stack
    // Drive forward, grab bottom ring

    // Turn to corner stack
    // Drive forward, pull top ring off with arm
    // Intake ring
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