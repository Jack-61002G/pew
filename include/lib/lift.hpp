#pragma once
#include "StateMachine.hpp"
#include "lib/TaskWrapper.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include <memory>

namespace lib {

enum class LiftState { MOVE, IDLE };

  //pid constant struct
  struct PIDConstants {
    double kP;
    double kI;
    double kD;
  };

class Lift : public StateMachine<LiftState, LiftState::IDLE>, public ryan::TaskWrapper {

private:
  std::shared_ptr<pros::MotorGroup> motors;
  
  const double gearRatio;

  const PIDConstants constants;

  double target;

  bool moving = false;


public:
  Lift(pros::MotorGroup *motors, double gearRatio, PIDConstants constants) : motors(motors), gearRatio(gearRatio), constants(constants) {
    motors->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    motors->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  }

  void setAngle(double angle);
  void waitUntilSettled();
  void loop() override;
};

} // namespace lib
// namespace lib