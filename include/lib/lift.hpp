#pragma once
#include "StateMachine.hpp"
#include "lib/TaskWrapper.hpp"
#include "pros/motors.hpp"

namespace lib {

enum class LiftState { MOVE, IDLE };

  //pid constant struct
  struct PIDConstants {
    double kP;
    double kI;
    double kD;
  };

class Lift : public StateMachine<LiftState, LiftState::IDLE>,
              public ryan::TaskWrapper {

private:
  pros::Motor motors;
  
  const double gearRatio;

  const PIDConstants constants;

  double target;

  bool moving = false;


public:
  Lift(pros::Motor motor, double gearRatio, PIDConstants constants) : motors(motor), gearRatio(gearRatio), constants(constants) {  motors.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);}

  void setAngle(double angle);
  void waitUntilSettled();
  void loop() override;
};

} // namespace lib
// namespace lib