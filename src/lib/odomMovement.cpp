#include "lib/chassis.h"
#include "point.hpp"
#include "pros/rtos.hpp"
#include "util.h"

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

void Chassis::followTrajectory(const std::vector<ProfilePoint>& trajectory, double maxSpeed, double b, double zeta, bool async) {
    if (async) {
        while (this->getState() == DriveState::MOVING) {
            pros::delay(10);
        }
        pros::Task task([=]() { followTrajectory(trajectory, b, zeta, false); });
        return;
    }

    state = DriveState::MOVING;
    
    // Ramsete constants
    const double kB = b;        // Usually 2.0
    const double kZeta = zeta;  // Usually 0.7
    
    // Get initial distance for relative tracking
    double startDistance = track->getDistance();
    
    // Index to track progress through trajectory
    size_t trajectoryIndex = 0;

    while (trajectoryIndex < trajectory.size()) {
        // Get current robot state
        Point currentPose = getPose();
        double currentX = currentPose.x;
        double currentY = currentPose.y;
        double currentTheta = currentPose.theta * M_PI / 180.0; // Convert to radians
        
        // Calculate distance traveled along path
        double distanceTraveled = track->getDistance() - startDistance;
        
        // Find the closest point in trajectory based on distance
        while (trajectoryIndex + 1 < trajectory.size() && 
               distanceTraveled > trajectory[trajectoryIndex].distance) {
            trajectoryIndex++;
        }

        // Get desired state from trajectory
        const ProfilePoint& desired = trajectory[trajectoryIndex];
        double desiredX = desired.position.x;
        double desiredY = desired.position.y;
        
        // Calculate desired heading based on the trajectory
        double desiredTheta;
        if (trajectoryIndex < trajectory.size() - 1) {
            const ProfilePoint& nextPoint = trajectory[trajectoryIndex + 1];
            desiredTheta = std::atan2(nextPoint.position.y - desired.position.y,
                                    nextPoint.position.x - desired.position.x);
        } else {
            // Use the last known heading for the final point
            desiredTheta = std::atan2(desired.position.y - currentY,
                                    desired.position.x - currentX);
        }

        // Transform error to robot frame
        double dx = desiredX - currentX;
        double dy = desiredY - currentY;
        double dtheta = constrain180(desiredTheta - currentTheta * 180.0 / M_PI) * M_PI / 180.0;

        // Error in robot's reference frame
        double error_x = std::cos(currentTheta) * dx + std::sin(currentTheta) * dy;
        double error_y = -std::sin(currentTheta) * dx + std::cos(currentTheta) * dy;
        double error_theta = dtheta;

        // Get desired velocity from the profile
        double desiredVelocity = desired.velocity;

        // Ramsete control law
        double k = 2 * kZeta * std::sqrt(desiredVelocity * desiredVelocity + kB * error_x * error_x);
        
        // Calculate linear and angular velocity commands
        double v = desiredVelocity * std::cos(error_theta) + k * error_x;
        double omega = desired.curvature * desiredVelocity + 
                      kB * desiredVelocity * std::sin(error_theta) / error_theta * error_y +
                      k * error_theta;

        // Convert to left and right wheel velocities
        double track_width = 12.0;
        double leftVelocity = v - omega * track_width / 2.0;
        double rightVelocity = v + omega * track_width / 2.0;

        // Scale velocities to match the profile's left/right velocities
        double scaleFactor = std::min(desired.leftVelocity / leftVelocity,
                                    desired.rightVelocity / rightVelocity);
        leftVelocity *= scaleFactor;
        rightVelocity *= scaleFactor;

        // Apply velocities to motors
        leftMotors->move_velocity((leftVelocity / maxSpeed) * 600);
        rightMotors->move_velocity((rightVelocity / maxSpeed) *600);

        pros::delay(25);

        // Check if we've reached the end of the trajectory
        if (trajectoryIndex >= trajectory.size() - 1 &&
            std::abs(error_x) < 0.5 && 
            std::abs(error_y) < 0.5 &&
            std::abs(error_theta) < 0.05) { // ~3 degrees tolerance
            break;
        }
    }

    // Stop motors and cleanup
    leftMotors->move(0);
    rightMotors->move(0);
    leftMotors->brake();
    rightMotors->brake();
    state = DriveState::IDLE;
}