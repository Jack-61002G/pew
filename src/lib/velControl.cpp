#include "lib/velControl.h"

double lib::velController::step(double position, double velocity,
                                double currentPos, double currentVel) {

  return power = (kV * velocity + kP_Pos * (position - currentPos) +
                  kP_Vel * (velocity - currentVel));
}

double lib::velController::getTargetPower() const { return power; }