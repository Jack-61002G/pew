#include "lib/chassis.h"
#include "point.hpp"
#include "pros/rtos.hpp"
#include "robodash/views/console.hpp"
#include "robotconfig.h"
#include "util.h"
#include <string>
#include <utility>

using namespace lib;

void Chassis::moveToPoint(float x, float y, PID linearPid, PID turningPid,
                          bool backwards, float maxSpeed, bool async) {

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() {
      moveToPoint(x, y, linearPid, turningPid, backwards, maxSpeed);
    });
  }

  state = DriveState::MOVING;

  // turn to the point
  double angle = fmod(
      radiansToDegrees(M_PI_2 - atan2(y - getPose().y, x - getPose().x)), 360);
  std::cout << angle << "\n";

  if (backwards) {
    angle += 180;
  }
  turn(angle, turningPid, maxSpeed, false);

  // move to the point
  float distance = sqrt(pow(x - getPose().x, 2) + pow(y - getPose().y, 2));
  std::cout << distance << "\n";

  if (backwards) {
    distance = -distance;
  }
  move(distance, linearPid, turningPid, maxSpeed, false);

  state = DriveState::IDLE;
}

void Chassis::followTrajectory(const std::vector<ProfilePoint>& trajectory, bool backwards, double maxSpeed, double b, double zeta, bool async) {
    state = DriveState::MOVING;
    std::cout << "Starting trajectory following (backwards = " << (backwards ? "true" : "false") << ")\n";
   
    double startDistance = track->getDistance();
    size_t trajectoryIndex = 1;
    
    while (trajectoryIndex < trajectory.size()) {
        Point currentPose = getPose();
        double distanceTraveled = std::abs(track->getDistance() - startDistance);
       
        std::cout << "Current pose: x=" << currentPose.x
                 << " y=" << currentPose.y
                 << " theta=" << currentPose.theta << "\n";
       
        while (distanceTraveled > trajectory[trajectoryIndex].distance) {
            trajectoryIndex++;
            if (trajectoryIndex >= trajectory.size()) break;
        }
        
        const ProfilePoint& desired = trajectory[trajectoryIndex];
       
        std::cout << "Profile velocities: left=" << desired.leftVelocity
                 << " right=" << desired.rightVelocity << "\n";
                 
        double leftVel = desired.leftVelocity;
        double rightVel = desired.rightVelocity;
        
        if (backwards) {
            leftVel = -leftVel;
            rightVel = -rightVel;
        }
        
        int leftMotorVel = static_cast<int>((leftVel / maxSpeed) * 600);
        int rightMotorVel = static_cast<int>((rightVel / maxSpeed) * 600);
        
        std::cout << "Motor velocities: left=" << leftMotorVel
                 << " right=" << rightMotorVel << "\n\n";
                 
        leftMotors->move_velocity(leftMotorVel);
        rightMotors->move_velocity(rightMotorVel);
        pros::delay(25);
    }
    
    leftMotors->move(0);
    rightMotors->move(0);
    leftMotors->brake();
    rightMotors->brake();
    state = DriveState::IDLE;
}

void Chassis::moveToPose(Point endPoint, bool backwards) {
    Point startPose = getPose();
    
    // Convert angles to radians, adjusting for coordinate system where 0° is forward (+y)
    double startHeading = (!backwards) ? (90 - startPose.theta) * M_PI / 180.0 : (90 - startPose.theta + 180) * M_PI / 180.0;
    double endHeading = (!backwards) ? (90 - endPoint.theta) * M_PI / 180.0 : (90 - endPoint.theta + 180) * M_PI / 180.0;
    
    // Calculate distances for control point placement
    double totalDistance = std::sqrt(std::pow(endPoint.x - startPose.x, 2) +
                                   std::pow(endPoint.y - startPose.y, 2));
    
    // Place control points at 1/3 and 2/3 of the path length
    double controlDist = totalDistance / 3.0;
    
    // Calculate control points based on front-facing heading
    Point control1(
        startPose.x + controlDist * std::cos(startHeading),
        startPose.y + controlDist * std::sin(startHeading)
    );
    
    Point control2(
        endPoint.x - controlDist * std::cos(endHeading),
        endPoint.y - controlDist * std::sin(endHeading)
    );
    
    std::vector<Point> controlPoints = {
        startPose,
        control1,
        control2,
        endPoint
    };
    
    auto profile = generator.generateProfile(
        controlPoints,
        64.1,   // max velocity
        80.0,   // max acceleration
        50.0,   // max deceleration
        11.25   // track width
    );
    
    followTrajectory(profile, backwards);
    console.print(std::to_string(getPose().x) + " ");
    console.println(std::to_string(getPose().y));
}