#include "lib/chassis.h"



using namespace lib;



//Chassis with only vertical tracking wheel and imu
void Chassis::loop() {

  double lastAngle = imu->get_rotation() * M_PI / 180.0;
  double lastPosition = track->getDistance();

  while (true) {

    double rawAngle = imu->get_rotation() * M_PI / 180.0;
    double rawPosition = track->getDistance();

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

Point Chassis::getPose(bool radians) { 
  if (radians) {return currentPose;}
  else {return Point(currentPose.x, currentPose.y, currentPose.theta * 180.0 / M_PI); }
}



void Chassis::setPose(Point newPose, bool radians) {
   currentPose = newPose;
   if (!radians) {currentPose.theta *= M_PI / 180.0;}
   headingTarget = newPose.theta;
 }