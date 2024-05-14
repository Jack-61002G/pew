#pragma once
#include "pros/motors.hpp"

namespace lib {

class Diffy {

private:
  std::vector<pros::Motor> motors;
  int size;

public:
  Diffy(const std::vector<int> &ports);
  Diffy(const std::vector<int> &ports,
        const std::vector<pros::MotorGear> &gearset);

  void spinDiffy(double lvolt, double rvolt);
  void spinDiffy(std::vector<double> voltages);
  void spinDiffy(double voltage);
  void spinVelocity(double lvel, double rvel);
  void zero();
  std::vector<double> getDiffyPos();
  std::vector<double> getDiffyVel();
  void setBrakeMode(pros::MotorBrake brakeMode);
  void stop();
  std::vector<pros::Motor> getMotors();

};

} // namespace lib