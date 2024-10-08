#include "lib/lift.hpp"
#include "pros/abstract_motor.hpp"
#include "robotconfig.h"
#include <cstdint>
#include <iostream>

using namespace lib;

void Lift::loop() {

  uint32_t now = pros::millis();
  motors->tare_position();
  while (true) {
    switch (getState()) {

    case LiftState::Stored:

      if (target != DOWN_ANGLE) {
        pid.variables_reset();
        target = DOWN_ANGLE; 
      }
  
      break;

    case LiftState::Recieve:
      
        if (target != MID_ANGLE) {
          pid.variables_reset();
          target = MID_ANGLE;
        }
    
        break;

    case LiftState::Score:
              
                if (target != UP_ANGLE) {
                  pid.variables_reset();
                  target = UP_ANGLE;
                }
            
                break;
    }
    pid.target_set(target / gearRatio);
    double error = (target / gearRatio) - motors->get_position();
    motors->move(pid.compute_error(error,motors->get_position()));
    //std::cout<<error<<std::endl;

    
    pros::Task::delay_until(&now, 15);
  }
}

void Lift::itterateState(bool delta) {
  if (delta) {
    setState(static_cast<LiftState>((static_cast<int>(getState()) + 1) % 3));
  } else {
    setState(static_cast<LiftState>((static_cast<int>(getState()) + 2) % 3));
  }
}

float Lift::getAngle() {
  return target;
}