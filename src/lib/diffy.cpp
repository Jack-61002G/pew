#include "lib/diffy.h"
#include "pros/abstract_motor.hpp"
using namespace lib;


Diffy::Diffy(const std::vector<int> &ports) {
  for (int i = 0; i < ports.size(); i++) {
    motors.push_back(pros::Motor(ports[i]));
  }
  size = motors.size();
}


Diffy::Diffy(const std::vector<int> &ports, const std::vector<pros::MotorGear> &gearset) {
  for (int i = 0; i < ports.size(); i++) {
    motors.push_back(pros::Motor(std::abs(ports[i]), gearset[i]));
  }
  size = motors.size();
}


void Diffy::spinDiffy(double lvolt, double rvolt) {
  for (int i = 0; i < size; i++) {
    if (i < size / 2) {
      motors[i].move(lvolt);
    } else {
      motors[i].move_velocity(rvolt);
    }
  }
}


void Diffy::spinDiffy(std::vector<double> voltages) {
  for (int i = 0; i < size; i++) {
    motors[i].move(voltages[i]);
  }
}


void Diffy::zero() {
  for (int i = 0; i < size; i++) {
    motors[i].set_zero_position(0);
  }
}


std::vector<double> Diffy::getDiffyPos() {
  double left = 0;
  double right = 0;
  int half = size / 2;

  for (int i = 0; i < motors.size(); i++) {
    if (i < half) {
      left += motors[i].get_position();
    } else {
      right += motors[i].get_position();
    }
  }
  std::vector<double> positions = {left / half, right / half};
  return positions;
}


std::vector<double> Diffy::getDiffyVel() {
  double left = 0;
  double right = 0;
  int half = size / 2;

  for (int i = 0; i < motors.size(); i++) {
    if (i < half) {
      left += motors[i].get_actual_velocity();
    } else {
      right += motors[i].get_actual_velocity();
    }
  }
  std::vector<double> velocities = {left / half, right / half};
  return velocities;
}


void Diffy::setBrakeMode(pros::MotorBrake brakeMode) {
  for (int i = 0; i < size; i++) {
    motors[i].set_brake_mode(brakeMode);
  }
}


void Diffy::stop() {
  for (int i = 0; i < size; i++) {
    motors[i].move(0);
    motors[i].brake();
  }
}

std::vector<pros::Motor> Diffy::getMotors() {
  return motors;
}

void Diffy::spinVelocity(double lvel, double rvel) {
  for (int i = 0; i < size; i++) {
    if (i < size / 2) {
      motors[i].move_velocity(lvel);
    } else {
      motors[i].move_velocity(rvel);
    }
  }
}
