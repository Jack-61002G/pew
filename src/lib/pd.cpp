#include "lib/pd.h"

using namespace lib;

PDconstants::PDconstants(std::vector<double> constants) {
  setConstants(constants);
}

void PDconstants::setConstants(std::vector<double> constants) {
  kP = constants[0];
  kD = constants[1];

  if (constants.size() > 2) {
    errorThreshold = constants[2];
    velocityThreshold = constants[3];
  }
  else {
    errorThreshold = 0;
    velocityThreshold = 0;
  }
}

std::vector<double> PDconstants::getConstants() { return {kP, kD}; }

PD::PD(std::vector<double> constants) : constants(constants) {}

void PD::setTarget(double target) { this->target = target; }

double PD::getTarget() { return target; }

double PD::calculate(double input) {

  error = target - input;
  derivative = error - prevError;

  prevError = error;

  return constants.getConstants()[0] * error +
         constants.getConstants()[1] * derivative;
}

double PD::update(double error) {

  derivative = error - prevError;

  prevError = error;

  return constants.getConstants()[0] * error +
         constants.getConstants()[1] * derivative;
}