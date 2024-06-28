#include "lib/pid.h"
#include "lib/util.h"

PID::PID(float kP, float kI, float kD, float slewRate) : kP(kP), kI(kI), kD(kD), slewRate(slewRate){}

float PID::update(const float error) {
  // calculate integral
  integral += error;
  if (sgn(error) != sgn(prevError))
    integral = 0;

  // calculate derivative
  const float derivative = error - prevError;
  prevError = error;

  // calculate output
  output = slew(prevOutput, error * kP + integral * kI + derivative * kD, slewRate);
  prevOutput = output;

  return output;
}

void PID::reset() {
  integral = 0;
  prevError = 0;
}