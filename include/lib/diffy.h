#pragma once
#include "main.h"
#include <vector>

namespace lib {

class Diffy {

private:
  pros::motor_brake_mode_e returnBrakeType(char brakeMode);

protected:
  std::vector<pros::Motor> motors;
  int size;

public:
  Diffy(const std::vector<int> &ports);
  Diffy(const std::vector<int> &ports,
        const std::vector<pros::MotorGear> &gearset);

  void spinDiffy(double rvolt, double lvolt);
  void spinDiffy(std::vector<double> voltages);
  std::vector<double> getDiffy();
};

} // namespace lib
  // Input number split in half and put each half into a motor group