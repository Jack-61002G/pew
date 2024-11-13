#pragma once
#include "lib/TaskWrapper.hpp"
#include "lib/StateMachine.hpp"
#include "main.h"

namespace lib {

enum class IntakeState {In, Out, Jam, Idle};

class Intake : public StateMachine<IntakeState, IntakeState::Idle>, public ryan::TaskWrapper {

private:
  std::shared_ptr<pros::MotorGroup> motors;


public:
  Intake(pros::MotorGroup *motors) : motors(motors) {}
  void loop() override;

    };
  }