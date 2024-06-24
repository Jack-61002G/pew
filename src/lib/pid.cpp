#include "lib/pid.h"
#include "lib/util.h"

PID::PID(float kP, float kI, float kD) : kP(kP), kI(kI), kD(kD) {}

float PID::update(const float error) {
  // calculate integral
  integral += error;
  if (sgn(error) != sgn(prevError))
    integral = 0;

  // calculate derivative
  const float derivative = error - prevError;
  prevError = error;

  // calculate output
  return error * kP + integral * kI + derivative * kD;
}

void PID::reset() {
  integral = 0;
  prevError = 0;
}