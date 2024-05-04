#pragma once

#include <vector>
namespace lib {

class PDconstants {

private:
  double kP;
  double kD;
  double errorThreshold;
  double velocityThreshold;

public:
  /*
  kp, kd, errorThreshold, velocityThreshold
  */
  PDconstants(std::vector<double> constants);

  /*
  kp, kd, errorThreshold, velocityThreshold
  */
  void setConstants(std::vector<double> constants);

  /*
  kp, kd, errorThreshold, velocityThreshold
  */
  std::vector<double> getConstants();
};

class PD {

private:
  PDconstants constants;

  double derivative;
  double prevError;
  double target;
  double error;

public:
  PD(std::vector<double> constants);

  void setTarget(double target);

  double getTarget();

  double calculate(double input);

  double update(double error);

  void reset();
};
} // namespace lib