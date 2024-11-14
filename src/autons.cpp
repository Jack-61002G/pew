#include "autons.h"
#include "lib///lift.hpp"
#include "lib/intake.hpp"
#include "lib/point.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"



void exit_condition_defaults() {
  turning.exit_condition_set(75, 1, 150, 3, 300, 500);
  linear.exit_condition_set(150, .25, 300, .75, 300, 500);
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
  chassis.moveToPoint(-1, 42, linear, heading, false, 127, false, true);
  intake.setState(lib::IntakeState::In);

  pros::Task task([&]() {
    pros::delay(700); intake.setState(lib::IntakeState::Idle);
  });
  chassis.swing(-50, true, 0, swing, 127, false, false, true);
  

  pros::Task task2([&]() {
    pros::delay(750); clamp.extend();
  });
  chassis.moveToPoint(-12, 36, linear, heading, true, 65);
  intake.setState(lib::IntakeState::In);
  pros::delay(200);

  chassis.moveToPoint(16, 30, linear, heading, false, 100);
  chassis.turn(0, turning);
  chassis.moveToPoint(18, 48, linear, heading, false, 100);
  pros::delay(200);

  chassis.turn(-135, turning);
  chassis.moveToPoint(-38, 6, linear, heading);
  lift.setState(lib::LiftState::Score);
  pros::delay(500);
  intake.setState(lib::IntakeState::Idle);
  chassis.moveToPoint(-38, -14, linear, heading);
}

void mogoSide() {
  // do stuff here
};

void mogoRush() {
  chassis.moveToPoint(1, -35, linear, heading, true, 127, false, true);
  pros::Task task([&]() {
    pros::delay(850); clamp.extend();
  });
  chassis.moveToPoint(9, -46, linear, heading, true, 70);
  intake.setState(lib::IntakeState::In);
  
  pros::delay(300);
  pros::Task task2([&]() {
    pros::delay(900); intake.setState(lib::IntakeState::Idle);
  });
  chassis.moveToPoint(10, -24, linear, heading, false, 100, false);

  chassis.turn(80, turning);
  clamp.retract();

  chassis.turn(-90, turning);
  
  pros::Task task3([&]() {
    pros::delay(650); clamp.extend();
  });
  chassis.moveToPoint(29.5, -28.5, linear, heading, true, 70);
  intake.setState(lib::IntakeState::In);
  
  pros::Task taskonebillion([&]() {
    pros::delay(700); intake.setState(lib::IntakeState::Idle);
  });
  pros::delay(500);

  chassis.moveToPoint(16, 5, linear, heading);
  doinker.extend();
  
  chassis.moveToPoint(0, 10, linear, heading);

  chassis.turn(135, turning);
}



void redLeft() {
  chassis.team = 1;
  lights.team = 1;

  ringSide();
}
void blueLeft() {
  chassis.team = 2;
  lights.team = 2;

  mogoSide();
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



void redRight() {
  chassis.team = 1;
  lights.team = 1;

  mogoSide();
}
void blueRight() {
  chassis.team = 2;
  lights.team = 2;

  ringSide();
}