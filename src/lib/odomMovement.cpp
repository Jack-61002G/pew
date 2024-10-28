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

void Chassis::followTrajectory(const std::vector<ProfilePoint>& trajectory, double b, double zeta, bool async) {
    if (async) {
        while (this->getState() == DriveState::MOVING) {
            pros::delay(10);
        }
        pros::Task task([=]() { followTrajectory(trajectory, b, zeta, false); });
        return;
    }

    state = DriveState::MOVING;
    
    // Ramsete constants
    // b > 0 (aggressive convergence)
    // zeta ∈ (0,1) (damping coefficient)
    const double kB = b;        // Usually 2.0
    const double kZeta = zeta;  // Usually 0.7

    size_t trajectoryIndex = 0;
    double startTime = pros::millis() / 1000.0; // Convert to seconds

    while (trajectoryIndex < trajectory.size()) {
        double currentTime = pros::millis() / 1000.0 - startTime;
        
        // Find the closest point in trajectory based on time
        while (trajectoryIndex + 1 < trajectory.size() && 
               (currentTime * trajectory.back().velocity) > trajectory[trajectoryIndex].distance) {
            trajectoryIndex++;
        }

        // Get current robot state
        Point currentPose = getPose();
        double currentX = currentPose.x;
        double currentY = currentPose.y;
        double currentTheta = currentPose.theta * M_PI / 180.0; // Convert to radians

        // Get desired state from trajectory
        const ProfilePoint& desired = trajectory[trajectoryIndex];
        double desiredX = desired.position.x;
        double desiredY = desired.position.y;
        
        // Calculate desired heading from trajectory curvature
        double desiredTheta = std::atan2(desired.position.y - currentY, desired.position.x - currentX);
        double desiredVelocity = desired.velocity;
        
        // Transform error to robot frame
        double dx = desiredX - currentX;
        double dy = desiredY - currentY;
        double dtheta = constrain180(desiredTheta - currentTheta * 180.0 / M_PI) * M_PI / 180.0;

        // Error in robot's reference frame
        double error_x = std::cos(currentTheta) * dx + std::sin(currentTheta) * dy;
        double error_y = -std::sin(currentTheta) * dx + std::cos(currentTheta) * dy;
        
        // Calculate angular error term
        double error_theta = dtheta;

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

        // Apply velocities to motors
        // Assuming your motor controllers accept velocity commands
        leftMotors->move_velocity(leftVelocity);
        rightMotors->move_velocity(rightVelocity);

        pros::delay(10);

        // Check if we've reached the end of the trajectory
        if (trajectoryIndex >= trajectory.size() - 1 &&
            std::abs(error_x) < 0.05 &&  // 5cm tolerance
            std::abs(error_y) < 0.05 &&
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