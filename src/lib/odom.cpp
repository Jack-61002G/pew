#include "lib/odom.hpp"



using namespace lib;



//odom with only vertical tracking wheel and imu
void Odom::loop() {

  double lastAngle = imu->get_rotation() * M_PI / 180.0;
  double lastPosition = vertiTracker->getDistance();

  while (true) {

    double rawAngle = imu->get_rotation() * M_PI / 180.0;
    double rawPosition = vertiTracker->getDistance();

    // Calculate the change in sensor values
    float deltaVertical = rawPosition - lastPosition;
    float deltaImu = rawAngle - lastAngle;

    // Update the previous sensor values
    lastPosition = rawPosition;
    lastAngle = rawAngle;


    // Calculate change in x and y
    float deltaY = deltaVertical;

    // Calculate local x and y
    float localX = 0;
    float localY = deltaY;


    // Calculate global x and y
    currentPose.x += localY * sin(rawAngle);
    currentPose.y += localY * cos(rawAngle);
    currentPose.theta = rawAngle;

    //delay
    pros::delay(10);
  }
}

Point Odom::getPose(bool radians) { return currentPose; }



void Odom::setPose(Point newPose) { currentPose = newPose; }