#include "lib/intake.hpp"
#include "pros/rtos.hpp"
#include "robotconfig.h"



using namespace lib;



void Intake::loop() {

  int jamTimer = 0;
  uint32_t now;
  uint32_t jamStartTime = 0;

  while (true) {
    // Intake jam logic
    if (std::abs(motors->get_actual_velocity_all()[0]) < 10 && getState() != IntakeState::Idle && !pisstake.is_extended()) {
      if (jamStartTime == 0) {
        // First time detecting slow velocity
        jamStartTime = pros::millis();
      }

      // Calculate how long we've been in slow velocity state
      uint32_t jamTimer = pros::millis() - jamStartTime;

      if (armLoading) {
        if (jamTimer > 1000) {
          setState(IntakeState::Jam);
        }
      } else {
        if (jamTimer > 75) {
          setState(IntakeState::Jam);
        }
      }
    } else {
      // Reset the start time when velocity is normal
      jamStartTime = 0;
      jamTimer = 0;
    }


    if (chassis.team == 2) {
      if (color->get_hue() < 30 || color->get_hue() > 300) {
        sort_time = pros::millis();
      }
    } else {
      if (color->get_hue() > 180 && color->get_hue() < 330) {
        sort_time = pros::millis();
      }
    }

    if (!sort_override && pros::millis() - sort_time < 150) {
      sort->extend();
    } else {
      sort->retract();
    }


    switch (getState()) {

    case IntakeState::Idle:

      motors->move(0);

      break;

    case IntakeState::In:

      motors->move(127);

      break;

    case IntakeState::Out:

      motors->move(-127);
      break;

    case IntakeState::Jam:

      motors->move(-127);
      if (armLoading) {
        setState(IntakeState::Idle);
        break;
      } else {
        pros::delay(250);
      }
      setState(IntakeState::In);
      break;
    }

    pros::delay(10);
  }
}