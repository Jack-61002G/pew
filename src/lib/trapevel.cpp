#include "lib/motion/trapevel.h"

trapevel::trapevel(double UP, double DOWN, double ff, double kp) {
  this->k_UP = UP;
  this->k_DOWN = DOWN;
  this->ff = ff;
  this->kP = kp;
}

double trapevel::update(double velocity, double actual) {
  double acceleration = (velocity - prevVelocity) / 0.02;
  prevVelocity = velocity;
  if (acceleration > 0) {
    return k_UP * acceleration + ff * velocity + kP * (velocity - actual);
  } else {
    return k_DOWN * acceleration + ff * velocity + kP * (velocity - actual);
  }
}

void trapevel::reset() { prevVelocity = 0; }