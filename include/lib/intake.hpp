#pragma once
#include "lib/TaskWrapper.hpp"
#include "lib/StateMachine.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/optical.hpp"

namespace lib {

enum class IntakeState {In, Out, Jam, Idle};

class Intake : public StateMachine<IntakeState, IntakeState::Idle>, public ryan::TaskWrapper {

private:
  std::shared_ptr<pros::MotorGroup> motors;
  std::shared_ptr<pros::Optical> color;
  std::shared_ptr<pros::adi::Pneumatics> sort;
  int sort_time;


public:

  bool sort_override = false;

  Intake(pros::MotorGroup *motors, pros::Optical *color, pros::adi::Pneumatics *sort) : motors(motors), color(color), sort(sort) {
    color->set_led_pwm(100);
  }
  void loop() override;

  };
}