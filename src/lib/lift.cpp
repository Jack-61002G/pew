#include "lib/lift.hpp"
#include "liblvgl/llemu.hpp"
#include "velControl.h"

using namespace lib;

void Lift::waitUntilSettled() {
  while (getState() != LiftState::IDLE) {
    pros::delay(15);
  }
}

void Lift::setAngle(double angle) {

  setState(LiftState::IDLE);
  pros::lcd::set_text(1, "setting");

  double target = (angle / 6) / gearRatio;

  profile = profiler->generateProfile(target);

  setState(LiftState::MOVE);
}

void Lift::loop() {

  uint32_t now = pros::millis();
  while (true) {
    switch (getState()) {

    case LiftState::IDLE:
      break;
      pros::lcd::set_text(1, "idle");

    case LiftState::MOVE:
    pros::lcd::set_text(1, "moving");
      for (auto &point : profile) {
        controller->step(point.second, point.first, motors->getDiffyPos()[0],
                         motors->getDiffyVel()[0]);
        motors->spinDiffy(controller->getTargetPower());
        pros::Task::delay_until(&now, 15);
      }
      setState(LiftState::HOLD);
      break;

    case LiftState::HOLD:
    pros::lcd::set_text(1, "holding");
      controller->step(profile.back().second, profile.back().first,
                       motors->getDiffyPos()[0], motors->getDiffyVel()[0]);
      motors->spinDiffy(controller->getTargetPower());
      break;
    }
    pros::Task::delay_until(&now, 15);
  }
}