#include "lib/chassis.h"
using namespace lib;

void lib::Chassis::startOdom(Odom *odom) {
  if (odom != nullptr) {
    odom->startTracking();
  }
}
//constructor
void Odom();

// if both encoders in the pair are nullptr -> use imu + motors

// if the first encoder in the pair is nullptr -> use second encoder as
// forwards/backwards +imu

// if the second encoder in the pair is nullptr -> use
//  first encoder as horizontal tracking + motors for forwards/backwards +imu

// if both encoders are not nullptr -> use both encoders + imu (no motor
// encoders)
void Odom::startTracking() {}

Point Odom::getPose(bool radians) { return currentPose; }

void Odom::setPose() {}