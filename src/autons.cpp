#include "lib/lift.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include "autons.h"



void left() {
    pros::Task task([&]() {
        pros::delay(640);
        clamp.extend();
    });

    chassis.move(-28.25, linear, heading, 1200, 85);
    intake.move(-127);

    pros::delay(300);

    // single stack
    chassis.turn(99, turning, 1000);
    chassis.move(24, linear, heading, 1000, 90);

    pros::delay(100);

    chassis.move(-15.5, linear, heading, 1000);

    // 4 stack
    chassis.turn(152, turning, 1000);
    chassis.move(17.6, linear, heading, 1000);
    pros::delay(650);
    chassis.move(-17.6, linear, heading, 1000);

    chassis.turn(90, turning, 1000);
    intake.move(0);
    chassis.move(19, linear, heading, 1000);

    chassis.turn(180, turning, 1000);
    intake.move(-127);
    chassis.move(17.75, linear, heading, 1000);

    pros::delay(700);
    chassis.move(-24, linear, heading, 1000);

    chassis.turn(250, turning, 1000);
    lift.setAngle(70);
    intake.move(0);

    chassis.move(24, linear, heading, 1000);
    clamp.retract();
    chassis.move(20, linear, heading, 1000);

    /*
    chassis.move(-40.5, linear, heading, 1000);
    intake.move(0);

    chassis.turn(50, turning, 1000);
    lift.setAngle(12.8);

    pros::delay(750);
    chassis.move(9, linear, heading, 1000, 50);

    lift.setAngle(17);

    pros::delay(500);
    claw.extend();

    pros::delay(250);
    chassis.move(-10, linear, heading, 500);
    */
}

void right() {
    
    pros::Task task([&]() {
        pros::delay(660);
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
    lift.setAngle(18.3);

    chassis.move(27, linear, heading, 1000, 90);
    chassis.move(9.53, linear, heading, 1000, 50);

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

    chassis.turn(39.8, turning, 500);

    pros::delay(1200);
    intake.move(0);

    chassis.move(9.57, linear, heading, 800, 80);

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

void AWP() {
    pros::Task task([&]() {
        pros::delay(620);
        clamp.extend();
    });

    chassis.move(-28.25, linear, heading, 1200, 85);
    intake.move(-127);

    pros::delay(300);

    // single stack
    chassis.turn(99, turning, 1000);
    chassis.move(24, linear, heading, 1000, 90);

    pros::delay(250);
    chassis.turn(-65, turning, 1000);
    pisstake.extend();
    chassis.move(44, linear, heading, 1000);

    pros::delay(200);
    pisstake.retract();
    pros::delay(200);
    chassis.move(-10, linear, heading, 1000);

    pros::delay(1400);
    intake.move(0);
    clamp.retract();
    
    chassis.move(6, linear, heading, 1000);
    chassis.turn(62, turning, 1000);

    pros::Task task2electricboogaloo([&]() {
        pros::delay(800);
        clamp.extend();
    });
    chassis.move(-36, linear, heading, 1000, 90);

    pros::delay(300);
    chassis.turn(-100, turning, 1000);
    intake.move(-127);
    chassis.move(26, linear, heading, 1000, 90);

    pros::delay(500);
    chassis.turn(90, turning, 1000);

    pros::delay(750);
    clamp.retract();
    intake.move(0);
    lift.setAngle(65);
    pros::delay(250);
  
    chassis.turn(107, turning, 1000);
    chassis.move(34, linear, heading, 1000);
}

void redLeft() {
    chassis.blueSide = false;
    left();
}

void redRight() {
    chassis.blueSide = false;
    right();
}

void redAWP() {
    chassis.blueSide = false;
    AWP();
}

void blueLeft() {
    chassis.blueSide = true;
    right(); // intentionally mirrored
}

void blueRight() {
    chassis.blueSide = true;
    left(); // intentionally mirrored
}

void blueAWP() {
    chassis.blueSide = true;
    AWP();
}