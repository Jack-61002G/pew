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
  PDconstants(std::vector<double> constants);

  void setConstants(std::vector<double> constants);

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

  void reset();
};
} // namespace lib