#include "lib///lift.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"
#include "autons.h"

void exit_condition_defaults() {
  turning.exit_condition_set(150, 1, 300, 3, 500, 500);
  linear.exit_condition_set(150, .25, 300, .75, 500, 500);
  swing.exit_condition_set(150, 1, 300, 3, 500, 500);
}

void left() { 
    pros::Task task([&]() {
        while (track.getDistance() > -22) { pros::delay(10);}
        clamp.extend();
    });
    
    chassis.move(-26.5, linear, heading, 1000, 55);

    pros::delay(250);
    intake.move(-127);

    pros::delay(300);

    // single stack
    chassis.turn(99, turning, 1000);
    chassis.move(24, linear, heading, 1000, 90);

    pros::delay(100);

    chassis.move(-16, linear, heading, 1000);

    // 4 stack
    chassis.turn(152, turning, 1000);
    chassis.move(18, linear, heading, 1000);
    pros::delay(700);
    chassis.move(-17.5, linear, heading, 1000);

    chassis.turn(90, turning, 1000);
    intake.move(0);
    chassis.move(17.5, linear, heading, 1000);

    chassis.turn(180, turning, 1000);
    intake.move(-127);
    chassis.move(16.5, linear, heading, 1000);

    pros::delay(750);
    chassis.move(-22.75, linear, heading, 1000);

    chassis.turn(250, turning, 1000);
    ////lift.setAngle(60);
    intake.move(0);

    chassis.move(37, linear, heading, 1000);
    clamp.retract();
    chassis.move(2, linear, heading, 1000);

    /*
    chassis.move(-40.5, linear, heading, 1000);
    intake.move(0);

    chassis.turn(50, turning, 1000);
    //lift.setAngle(12.8);

    pros::delay(750);
    chassis.move(9, linear, heading, 1000, 50);

    //lift.setAngle(17);

    pros::delay(500);
    claw.extend();

    pros::delay(250);
    chassis.move(-10, linear, heading, 500);
    */
}

void right() { // unused
    
    pros::Task task([&]() {
        while (track.getDistance() > -22) { pros::delay(10);}
        clamp.extend();
    });
    
    chassis.move(-26.5, linear, heading, 1000, 57);

    pros::delay(250);
    intake.move(-127);

    chassis.turn(-97, turning, 1200);
    chassis.move(22.5, linear, heading, 1200, 90);

    pros::delay(750);
    chassis.turn(-16, turning, 500);

    pros::delay(150);
    intake.move(0);
    //lift.setAngle(18.3);

    chassis.move(22.6, linear, heading, 1500, 90);
    chassis.move(10, linear, heading, 1200, 50);

    //claw.extend();
    pros::delay(200);
    //lift.setAngle(20);

    chassis.move(-10, linear, heading, 800);

    chassis.turn(120, turning, 800);

    pros::delay(250);
    //claw.retract();
    pros::delay(400);
    //lift.setState(lib::LiftState::DOWN_IN);

    chassis.turn(89, turning, 800);

    //pisstake.extend();
    intake.move(-127);
    chassis.move(45, linear, heading, 1200, 100);

    pros::delay(200);
    //pisstake.retract();

    pros::delay(200);
    chassis.move(-40, linear, heading, 1200, 100);
    chassis.turn(135, turning, 1000);

    intake.move(0);

    pros::delay(500);
    //lift.setAngle(60);

    chassis.move(34, linear, heading, 1200, 100);
    clamp.retract();
    chassis.move(8, linear, heading, 1000);

}

void AWP() {
    pros::Task task([&]() {
        while (track.getDistance() > -22) { pros::delay(10);}
        clamp.extend();
    });
    
    chassis.move(-26.5, linear, heading, 1000, 55);

    pros::delay(250);
    intake.move(-127);

    // single stack
    chassis.turn(99, turning, 1000);
    chassis.move(23.5, linear, heading, 1000, 90);

    pros::delay(250);
    chassis.turn(-60, turning, 1000);
    //pisstake.extend();
    chassis.move(44.3, linear, heading, 1000);

    pros::delay(200);
    //pisstake.retract();
    pros::delay(200);
    chassis.move(-10, linear, heading, 1000);

    pros::delay(1400);
    intake.move(0);
    clamp.retract();
    
    chassis.move(9, linear, heading, 1000);
    chassis.turn(59.25, turning, 1000);

    // second mogo
    float startPos = track.getDistance();
    pros::Task task2electricboogaloo([&]() {
        while (track.getDistance() - startPos > -30) { pros::delay(10);}
        clamp.extend();
    });
    chassis.move(-38.25, linear, heading, 1200, 81);

    pros::delay(300);
    chassis.turn(-86, turning, 1000);
    intake.move(-127);
    chassis.move(17.5, linear, heading, 1000, 92.5);

    pros::delay(600);
    chassis.turn(90, turning, 1000);
    chassis.move(24, linear, heading, 1000);

    pros::delay(400);
    clamp.retract();
    intake.move(0);
    //lift.setAngle(60);
    pros::delay(400);
    
    chassis.turn(135, turning, 1000);
    chassis.move(10, linear, heading, 1000);
}

void redLeft() {
    chassis.blueSide = false;
    left();
}

void redRight() {
    chassis.blueSide = false;

    pros::Task task([&]() {
        while (track.getDistance() > -22) { pros::delay(10);}
        clamp.extend();
    });
    
    chassis.move(-26.5, linear, heading, 1000, 57);

    pros::delay(250);
    intake.move(-127);

    chassis.turn(-97, turning, 1200);
    chassis.move(22.5, linear, heading, 1200, 90);

    pros::delay(750);
    chassis.turn(-16, turning, 500);

    pros::delay(150);
    intake.move(0);
    //lift.setAngle(18.3);

    chassis.move(22.6, linear, heading, 1500, 90);
    chassis.move(10, linear, heading, 1200, 50);

    //claw.extend();
    pros::delay(200);
    //lift.setAngle(20);

    chassis.move(-10, linear, heading, 800);

    chassis.turn(120, turning, 800);

    pros::delay(250);
    //claw.retract();
    pros::delay(400);
    //lift.setState(lib::LiftState::DOWN_IN);

    chassis.turn(89, turning, 800);

    //pisstake.extend();
    intake.move(-127);
    chassis.move(45, linear, heading, 1200, 100);

    pros::delay(200);
    //pisstake.retract();

    pros::delay(200);
    chassis.move(-40, linear, heading, 1200, 100);
    chassis.turn(135, turning, 1000);

    intake.move(0);

    pros::delay(500);
    ////lift.setAngle(60);

    chassis.move(34, linear, heading, 1200, 100);
    clamp.retract();
    chassis.move(2, linear, heading, 1000);

}

void redAWP() {
    chassis.blueSide = false;
    AWP();
}

void blueLeft() {
    chassis.blueSide = true;

    pros::Task task([&]() {
        while (track.getDistance() > -22) { pros::delay(10);}
        clamp.extend();
    });
    
    chassis.move(-26.5, linear, heading, 1000, 57);

    pros::delay(250);
    intake.move(-127);

    chassis.turn(-97, turning, 1200);
    chassis.move(22.5, linear, heading, 1200, 90);

    pros::delay(750);
    chassis.turn(0, turning, 500);
    chassis.move(20, linear, heading, 1200, 100);
    chassis.turn(-60, turning, 1000);

    intake.move(0);
    //lift.setState(lib::LiftState::UP_IN);

    chassis.move(14.3, linear, heading, 1200, 50);

    //claw.extend();
    //pros::delay(200);
    ////lift.setState(lib::LiftState::UP_IN);

    chassis.move(-10, linear, heading, 800);

    chassis.turn(60, turning, 800);

    pros::delay(250);
    //claw.retract();
    //pros::delay(400);
    ////lift.setState(lib::LiftState::DOWN_IN);

    chassis.turn(89, turning, 800);

    //pisstake.extend();
    intake.move(-127);
    chassis.move(40, linear, heading, 1200, 100);

    pros::delay(200);
    //pisstake.retract();

    pros::delay(200);
    chassis.move(-35, linear, heading, 1200, 100);
    chassis.turn(135, turning, 1000);

    intake.move(0);

    pros::delay(500);
    ////lift.setAngle(60);

    chassis.move(30, linear, heading, 1200, 100);
    clamp.retract();
    chassis.move(2, linear, heading, 1000);

}

void blueRight() {
    chassis.blueSide = true;
    left(); // intentionally mirrored
}

void blueAWP() {
    chassis.blueSide = true;
    AWP();
}