#include "lib/diffy.hpp"
#include "pros/motors.hpp"

using namespace lib;

void Diffy::loop() {
  while (true) {
    switch (getState()) {
    case DiffyState::ArmUpIntakeIn:
      if (topSwitch->get_value()) {
        setState(DiffyState::ArmIdleIntakeIn);
      } else {
        leftMotor->move(127);
        rightMotor->move(50);
      }
      break;

    case DiffyState::ArmUpIntakeOut:
      if (topSwitch->get_value()) {
        setState(DiffyState::ArmIdleIntakeOut);
      } else {
        leftMotor->move(50);
        rightMotor->move(127);
      }
      break;

    case DiffyState::ArmUpIntakeIdle:
      if (topSwitch->get_value()) {
        setState(DiffyState::ArmIdleIntakeIdle);
      }
      leftMotor->move(-127);
      rightMotor->move(-127);
      break;

    case DiffyState::ArmDownIntakeIn:
      if (bottomSwitch->get_value()) {
        setState(DiffyState::ArmIdleIntakeIn);
      } else {
        leftMotor->move(-50);
        rightMotor->move(-127);
      }
      break;

    case DiffyState::ArmDownIntakeOut:
      if (bottomSwitch->get_value()) {
        setState(DiffyState::ArmIdleIntakeOut);
      } else {
        leftMotor->move(-127);
        rightMotor->move(-50);
      }
      break;

    case DiffyState::ArmDownIntakeIdle:
      if (bottomSwitch->get_value()) {
        setState(DiffyState::ArmIdleIntakeIdle);
      }
      leftMotor->move(127);
      rightMotor->move(127);
      break;

    case DiffyState::ArmIdleIntakeIn:
      leftMotor->move(127);
      rightMotor->move(-127);
      break;

    case DiffyState::ArmIdleIntakeOut:
      leftMotor->move(-127);
      rightMotor->move(127);
      break;

    case DiffyState::ArmIdleIntakeIdle:
      leftMotor->move(0);
      rightMotor->move(0);
      break;
    }
    pros::Task::delay(20);
  }
}