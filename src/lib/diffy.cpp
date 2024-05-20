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

void Diffy::spinDiffy(double voltage) {
  for (int i = 0; i < size; i++) {
    motors[i].move(voltage);
  }
}


void Diffy::zero() {
  for (int i = 0; i < size; i++) {
    motors[i].set_zero_position(0);
  }
}


std::vector<double> Diffy::getDiffyPos() {
  std::vector<double> positions; // Declare the 'positions' vector

  for (int i = 0; i < size; i++) {
    positions.push_back(motors[i].get_position());
  }

  return positions; // Return the 'positions' vector
}


std::vector<double> Diffy::getDiffyVel() {
  int half = size / 2;

  return {0};
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
      motors[i].move(lvel);
    } else {
      motors[i].move(rvel);
    }
  }
}
