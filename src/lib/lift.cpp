#include "lib/lift.hpp"
#include "pros/abstract_motor.hpp"
#include <cstdint>
#include <iostream>

using namespace lib;

void Lift::loop() {

  uint32_t now = pros::millis();
  while (true) {
    switch (getState()) {

    case LiftState::UP_OUT:

      if (target != UP_ANGLE) {
        pid.reset();
        target = UP_ANGLE; 
      }
      wrist->set_value(true);
  
      break;

    case LiftState::UP_IN:
      
        if (target != UP_ANGLE) {
          pid.reset();
          target = UP_ANGLE;
        }
        wrist->set_value(false);
    
        break;

    case LiftState::DOWN_OUT:
        
          if (target != DOWN_ANGLE) {
            pid.reset();
            target = DOWN_ANGLE;
          }
          wrist->set_value(true);
      
          break;

    case LiftState::DOWN_IN:
          
            if (target != DOWN_ANGLE) {
              pid.reset();
              target = DOWN_ANGLE;
            }
            wrist->set_value(false);
        
            break;

    case LiftState::OUT_CUSTOM:

              wrist->set_value(true);
          
              break;

    case LiftState::MID_IN:
              
                if (target != MID_ANGLE) {
                  pid.reset();
                  target = MID_ANGLE;
                }
                wrist->set_value(false);
            
                break;

    float error = target - motors->get_position() / gearRatio;
    motors->move(pid.update(error));


    
    }

    
    pros::Task::delay_until(&now, 15);
  }
}

void Lift::setAngle(float angle) {
  setState(LiftState::OUT_CUSTOM);
  pid.reset();
  target = angle;
}