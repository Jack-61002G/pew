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


}

void redRight() {
    // Back up to right side mogo
    // clamp mogo

    // Turn to stack
    // Drive forward, grab bottom ring

    // Turn to corner stack
    // Drive forward, pull top ring off with arm
    // Intake ring
}

void redRush() {
    // Back up to mid mogo
    // put preoad on mogo
    // drop mogo

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