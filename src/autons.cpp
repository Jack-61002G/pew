#include "autons.h"
#include "lib///lift.hpp"
#include "lib/point.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"

void exit_condition_defaults() {
  turning.exit_condition_set(150, 1, 300, 3, 500, 500);
  linear.exit_condition_set(150, .25, 300, .75, 500, 500);
  swing.exit_condition_set(75, 2, 150, 6, 150, 500);
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

  intake.move(127);
  pros::delay(750);
  intake.move(0);

  chassis.move(6, linear, heading);

  pros::Task task([&]() {pros::delay(1050); clamp.extend();});

  chassis.moveToPoint(-25, 16, linear, heading, true, 80);

  pros::delay(250);
  intake.move(127);
  chassis.moveToPoint(-24, 38, linear, heading, false, 80);

  pros::delay(500);
  lift.setState(lib::LiftState::Recieve);

  chassis.moveToPoint(-55, 68, linear, heading, false, 90);
  pros::delay(500);
  intake.move(0);

  chassis.moveToPoint(-64, 68, linear, heading);
  lift.setState(lib::LiftState::Score);

  pros::delay(300);
  chassis.move(-10, linear, heading);
  lift.setState(lib::LiftState::Stored);
  
  intake.move(127);
  chassis.moveToPoint(-48, 4, linear, heading, false, 55);
  chassis.move(-12, linear, heading);
  chassis.moveToPoint(-65, 16, linear, heading);
  chassis.moveToPoint(-71, 1, linear, heading, true);
  clamp.retract();
  intake.move(0);
  chassis.move(10, linear, heading);

  chassis.moveToPoint(6, 15, linear, heading, true);
  pros::Task task2electricboogaloo([&]() {pros::delay(500); clamp.extend();});
  chassis.moveToPoint(25, 15.5, linear, heading, true, 80);

  pros::delay(250);
  intake.move(127);
  chassis.moveToPoint(24, 38, linear, heading, false, 80);

  pros::delay(500);
  lift.setState(lib::LiftState::Recieve);

  chassis.moveToPoint(55, 68, linear, heading, false, 90);
  pros::delay(500);
  intake.move(0);

  chassis.moveToPoint(64 , 68, linear, heading);
  lift.setState(lib::LiftState::Score);

  pros::delay(300);
  chassis.move(-10, linear, heading);
  lift.setState(lib::LiftState::Stored);
  
  intake.move(127);
  chassis.moveToPoint(48, 4, linear, heading, false, 55);
  chassis.move(-12, linear, heading);
  chassis.moveToPoint(65, 16, linear, heading);
  chassis.moveToPoint(71, 1, linear, heading, true);
  clamp.retract();

  chassis.moveToPoint(48, 72, linear, heading);
  chassis.moveToPoint(24, 94, linear, heading);
  intake.move(0);
  pros::Task taskthethird([&]() {pros::delay(1600); clamp.extend();});
  chassis.moveToPoint(0, 110, linear, heading, true, 80);

}

void redLeft() {
  chassis.moveToPoint(0, 0, linear, heading);
  clampAt(24);
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