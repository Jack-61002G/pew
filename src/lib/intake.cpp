#include "lib/intake.hpp"
#include "pros/rtos.hpp"
using namespace lib;
void Intake::loop() {

  int jamTimer = 0;
  uint32_t now;
  uint32_t jamStartTime = 0;

  while (true) {

    // Then in your logic:
    if (std::abs(motors->get_actual_velocity_all()[0]) < 10 && getState() != IntakeState::Idle) {
      if (jamStartTime == 0) {
        // First time detecting slow velocity
        jamStartTime = pros::millis();
      }

      // Calculate how long we've been in slow velocity state
      uint32_t jamTimer = pros::millis() - jamStartTime;

      if (jamTimer > 75) {
        setState(IntakeState::Jam);
      }
    } else {
      // Reset the start time when velocity is normal
      jamStartTime = 0;
      jamTimer = 0;
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
      pros::delay(250);
      setState(IntakeState::In);
      break;
    }

    pros::delay(25);
  }
}