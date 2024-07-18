#include "lib/chassis.h"
using namespace lib;

void lib::Chassis::startOdom(Odom *odom) {
  if (odom != nullptr) {
    odom->startTracking();
  }
}
//constructor
void Odom();

//odom with only vertical tracking wheel and imu
void Odom::startTracking() {

  double lastAngle = imu->get_rotation();
  double lastPosition = vertiTracker->getDistance();

  while (true) {

    double currentAngle = imu->get_rotation();
    double currentPosition = vertiTracker->getDistance();

    double deltaAngle = currentAngle - lastAngle;
    double deltaPosition = currentPosition - lastPosition;

    //calculate the change in x and y
    double deltaX = deltaPosition * cos(deltaAngle);
    double deltaY = deltaPosition * sin(deltaAngle);

    //update the current pose
    currentPose.x += deltaX;
    currentPose.y += deltaY;
    currentPose.theta = currentAngle;

    //update the last angle and position
    lastAngle = currentAngle;
    lastPosition = currentPosition;

    //delay
    pros::delay(10);
  }

}

Point Odom::getPose(bool radians) { return currentPose; }
void Odom::setPose(Point newPose) { currentPose = newPose; }