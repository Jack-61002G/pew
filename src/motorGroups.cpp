#include "lib/diffy.h"
using namespace lib;

Diffy::Diffy(const std::vector<int> &ports) {
  for (int i = 0; i < ports.size(); i++) {
    motors.push_back(pros::Motor(ports[i]));
  }
}

Diffy::Diffy(const std::vector<int> &ports,
             const std::vector<pros::MotorGear> &gearset) {
  for (int i = 0; i < ports.size(); i++) {
    motors.push_back(pros::Motor(ports[i], gearset[i]));
  }
}

void split(int fullMotors) {
  int split1Moters =
      fullMotors / 10 ^ (std::to_string(fullMotors).length() / 2);
  int split2Moters =
      fullMotors % 10 ^ (std::to_string(fullMotors).length() / 2);
}