#pragma once
#include "StateMachine.hpp"
#include "lib/TaskWrapper.hpp"
#include "pid.h"
#include "pros/adi.hpp"
#include "pros/motor_group.hpp"
#include <memory>

namespace lib {

enum class LiftState {UP_OUT, UP_IN, DOWN_OUT, DOWN_IN, OUT_CUSTOM, MID_IN};

  //pid constant struct
  struct PIDConstants {
    double kP;
    double kI;
    double kD;
  };

class Lift : public StateMachine<LiftState, LiftState::DOWN_IN>, public ryan::TaskWrapper {

private:
  std::shared_ptr<pros::MotorGroup> motors;
  std::shared_ptr<pros::adi::Pneumatics> wrist;
  const float DOWN_ANGLE = 0;
  const float MID_ANGLE = 45;
  const float UP_ANGLE = 81 ;

  const float gearRatio;

  const PIDConstants constants;

  double target;

  PID pid = PID(constants.kP, constants.kI, constants.kD);

public:
  Lift(pros::MotorGroup *motors, pros::adi::Pneumatics *wrist, double gearRatio, PIDConstants constants) : motors(motors), wrist(wrist), gearRatio(gearRatio), constants(constants) {
    motors->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    motors->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  }
  void loop() override;

  void setAngle(float angle);
  
};

} // namespace lib
// namespace lib