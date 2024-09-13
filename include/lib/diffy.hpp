#include "lib/StateMachine.hpp"
#include "lib/TaskWrapper.hpp"
#include "pros/adi.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"

namespace lib {

enum class DiffyState {
  ArmUpIntakeIn,
  ArmUpIntakeOut,
  ArmUpIntakeIdle,
  ArmDownIntakeIn,
  ArmDownIntakeOut,
  ArmDownIntakeIdle,
  ArmIdleIntakeIn,
  ArmIdleIntakeOut,
  ArmIdleIntakeIdle
};

class Diffy : public StateMachine<DiffyState, DiffyState::ArmIdleIntakeIdle>,
              public ryan::TaskWrapper {

private:
  std::shared_ptr<pros::Motor> leftMotor;
  std::shared_ptr<pros::Motor> rightMotor;
  std::shared_ptr<pros::adi::DigitalIn> bottomSwitch;
  std::shared_ptr<pros::adi::DigitalIn> topSwitch;

public:
  Diffy(pros::Motor leftMotor, pros::Motor rightMotor, pros::adi::DigitalIn *bottomSwitch,
        pros::adi::DigitalIn *topSwitch)
        : leftMotor(std::make_shared<pros::Motor>(leftMotor)),
          rightMotor(std::make_shared<pros::Motor>(rightMotor)),
          bottomSwitch(std::make_shared<pros::adi::DigitalIn>(*bottomSwitch)),
          topSwitch(std::make_shared<pros::adi::DigitalIn>(*topSwitch)) {}

  void loop() override;
};

} // namespace lib