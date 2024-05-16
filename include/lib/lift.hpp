#pragma once
#include "StateMachine.hpp"
#include "diffy.h"
#include "lib/TaskWrapper.hpp"
#include "profiler.hpp"
#include "velControl.h"

namespace lib {

enum class LiftState { MOVE, HOLD, IDLE };

class Lift : public StateMachine<LiftState, LiftState::IDLE>,
              public ryan::TaskWrapper {

private:
  lib::Diffy *motors;
  const profileConstraints constraints;
  velController *controller;
  const double gearRatio;

  Profiler *profiler;
  std::vector<std::pair<double, double>> profile;

public:
  Lift(lib::Diffy *motors, profileConstraints constraints, velController *controller, double gearRatio = 1)
      : motors(motors), constraints(constraints), controller(controller), gearRatio(gearRatio) {
    profiler = new Profiler(constraints);
  }

  void setAngle(double angle);
  void waitUntilSettled();
  void loop() override;
};

} // namespace lib
// namespace lib