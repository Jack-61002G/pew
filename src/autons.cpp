#include "autons.h"
#include "lib///lift.hpp"
#include "lib/intake.hpp"
#include "lib/point.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"



void exit_condition_defaults() {
  turning.exit_condition_set(75, 1.5, 150, 4, 300, 500);
  linear.exit_condition_set(150, .33, 300, 1, 300, 500);
  swing.exit_condition_set(75, 2, 150, 6, 300, 500);
}



void clampAt(float distance) {
  pros::Task task([&]() {
    float startDistance = track.getDistance();
    while (fabs(startDistance - track.getDistance()) < distance) {
      pros::delay(15);
    }
    clamp.extend();
  });
}



void skills() {

  intake.setState(lib::IntakeState::In);
  pros::delay(750);
  intake.setState(lib::IntakeState::Idle);

  chassis.move(8, linear, heading);

  pros::Task task([&]() {pros::delay(1300); clamp.extend();});

  chassis.moveToPoint(-25, 16, linear, heading, true, 70);

  pros::delay(250);
  intake.setState(lib::IntakeState::In);
  chassis.moveToPoint(-24, 38, linear, heading, false, 80);

  pros::delay(750);
  lift.setState(lib::LiftState::Recieve);

  chassis.moveToPoint(-59, 68, linear, heading, false, 80);
  pros::delay(500);
  chassis.move(-9, linear, heading);
  intake.setState(lib::IntakeState::Idle);

  chassis.moveToPoint(-64, 63, linear, heading);
  lift.setState(lib::LiftState::Score);

  pros::delay(300);
  chassis.move(-10, linear, heading);
  lift.setState(lib::LiftState::Stored);
  
  intake.setState(lib::IntakeState::In);
  chassis.moveToPoint(-48, 4, linear, heading, false, 55);
  chassis.move(-12, linear, heading);
  chassis.moveToPoint(-65, 16, linear, heading);
  chassis.moveToPoint(-71, 1, linear, heading, true);
  clamp.retract();
  intake.setState(lib::IntakeState::Idle);
  chassis.move(10, linear, heading);

  chassis.moveToPoint(6, 15, linear, heading, true);
  pros::Task task2electricboogaloo([&]() {pros::delay(500); clamp.extend();});
  chassis.moveToPoint(25, 15.5, linear, heading, true, 80);

  pros::delay(250);
  intake.setState(lib::IntakeState::In);
  chassis.moveToPoint(24, 38, linear, heading, false, 80);

  pros::delay(500);
  lift.setState(lib::LiftState::Recieve);

  chassis.moveToPoint(59, 68, linear, heading, false, 80);
  pros::delay(500);
  chassis.move(-11, linear, heading);
  intake.setState(lib::IntakeState::Idle);

  chassis.moveToPoint(64, 63.5, linear, heading);
  lift.setState(lib::LiftState::Score);

  pros::delay(300);
  chassis.move(-10, linear, heading);
  lift.setState(lib::LiftState::Stored);
  
  intake.setState(lib::IntakeState::In);
  chassis.moveToPoint(48, 4, linear, heading, false, 55);
  chassis.move(-12, linear, heading);
  chassis.moveToPoint(65, 16, linear, heading);
  chassis.moveToPoint(71, 1, linear, heading, true);
  clamp.retract();

  chassis.moveToPoint(48, 72, linear, heading);
  pros::Task taskthethird([&]() {pros::delay(1000); intake.setState(lib::IntakeState::Idle);});
  chassis.moveToPoint(24, 94, linear, heading);

  pros::Task revengeofthetasks([&]() {pros::delay(1650); clamp.extend();});
  chassis.moveToPoint(1.5, 113.5, linear, heading, true, 80);

  intake.setState(lib::IntakeState::In);
  chassis.moveToPoint(-24, 90, linear, heading, false, 80);
  chassis.moveToPoint(-48, 90, linear, heading, false, 80);

  chassis.moveToPoint(-48, 116, linear, heading, false, 60);
  chassis.move(-8, linear, heading);
  chassis.moveToPoint(-60, 114, linear, heading, false, 60);
  chassis.move(-10, linear, heading);
  intake.setState(lib::IntakeState::Idle);
  doinker.extend();
  chassis.moveToPoint(-60, 120, linear, heading);
  chassis.swing(135, true, 0, turning);
  chassis.move(-12, linear, heading);
}



void ringSide() {
  chassis.moveToPoint(-0.6, 40, linear, heading, false, 110, false, true);
  intake.setState(lib::IntakeState::In);

  pros::Task task([&]() {
    pros::delay(800); intake.setState(lib::IntakeState::Idle);
  });
  chassis.swing(-47, true, 0, swing, 127, false, true, true);
  
  pros::Task task2([&]() {
    pros::delay(770); clamp.extend();
  });
  chassis.moveToPoint(12, 33, linear, heading, true, 60);
  intake.setState(lib::IntakeState::In);

  chassis.moveToPoint(-16, 30, linear, heading, false);
  chassis.turn(0, turning, 127, false, true);
  chassis.moveToPoint(-18, 48, linear, heading, false);
  chassis.move(-6, linear, heading, 127, false, true);

  chassis.turn(135, turning, 127, false, true);

  intake.sort_override = true;
  sorter.extend();

  lift.setState(lib::LiftState::LowScore);
  chassis.moveToPoint(37.5, -2, linear, heading);

  chassis.turn(180, turning, 127, false, true);
  chassis.move(10, linear, heading, 127, false, true);
  chassis.move(-10, linear, heading, 127, false, true);
  chassis.turn(0, turning, 127, false, true, true);
  intake.setState(lib::IntakeState::Idle);
  lift.setState(lib::LiftState::Score);
  chassis.move(24, linear, turning, 100);
}



void mogoRush() {
  chassis.moveToPoint(-0.5, -33, linear, heading, true, 127, false, true);
  pros::Task task([&]() {
    pros::delay(700); clamp.extend();
  });
  chassis.moveToPoint(9, -47, linear, heading, true, 60);
  intake.setState(lib::IntakeState::In);
  
  pros::delay(300);
  pros::Task task2([&]() {
    pros::delay(900); intake.setState(lib::IntakeState::Idle);
  });
  chassis.moveToPoint(10, -27, linear, heading, false, 100, false);
  chassis.move(-3, linear, heading);

  chassis.turn(80, turning, 127, false, true);
  clamp.retract();

  chassis.turn(-90, turning, 127, false, true);
  
  pros::Task task3([&]() {
    pros::delay(680); clamp.extend();
  });
  chassis.moveToPoint(32, -29, linear, heading, true, 60);
  intake.setState(lib::IntakeState::In);
  
  pros::Task taskonebillion([&]() {
    pros::delay(750); intake.setState(lib::IntakeState::Idle);
  });
  pros::delay(500);

  if (chassis.team == 2) {
    chassis.moveToPoint(0, -8, linear, heading);
    doinker.extend();
    pros::delay(500);
  
    chassis.moveToPoint(-8, 10, linear, heading);
    
    chassis.turn(0, turning);
    chassis.turn(-90, turning);
  } else {
    chassis.moveToPoint(16, 0, linear, heading);
    doinker.extend();
    chassis.turn(10, turning);
    pros::delay(500);
  
    chassis.moveToPoint(0, 10, linear, heading);
    chassis.turn(135, turning);
  }

  doinker.retract();
  pros::delay(500);
  intake.setState(lib::IntakeState::In);
  chassis.turn(-45, turning, 127, false, true);
  chassis.moveToPoint(-14, 14, linear, heading);
  chassis.move(-12, linear, heading, 127, false, true);
  chassis.turn(-45, turning, 127, false, true);
  chassis.moveToPoint(-1, 18, linear, heading, true);
  clamp.retract();
  intake.setState(lib::IntakeState::Idle);
  chassis.move(12, linear, heading);
  chassis.turn(180, turning, 127, false, true);
}



void redRingSide() {
  chassis.team = 1;
  lights.team = 1;

  ringSide();
}
void blueRingSide() {
  chassis.team = 2;
  lights.team = 2;

  ringSide();
}



void redRush() {
  chassis.team = 1;
  lights.team = 1;

  mogoRush();
}
void blueRush() {
  chassis.team = 2;
  lights.team = 2;

  mogoRush();
}