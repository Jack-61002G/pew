#pragma once
#include "main.h"
#include "lib/odom.hpp"
#include "lib/pid.h"
#include "pros/motors.h"

namespace lib {

class Chassis {

private:
  enum class DriveState { IDLE, MOVING };
  std::shared_ptr<pros::MotorGroup> leftMotors;
  std::shared_ptr<pros::MotorGroup> rightMotors;
  std::shared_ptr<pros::Imu> imu;
  DriveState state;

  const int rpm;
  const double wheel;
  const double tpr;

  double angleWrap(double angle) {
    while (angle > 360) {
      angle -= 360;
    }
    while (angle < -360) {
      angle += 360;
    }
    return angle;
  }

public:
  DriveState getState() { return state; }

  // constructor
  Chassis(pros::MotorGroup *leftMotors, pros::MotorGroup *rightMotors, pros::Imu *imu, int rpm, double wheel)
      : leftMotors(leftMotors), rightMotors(rightMotors), imu(imu), rpm(rpm), wheel(wheel), tpr(50 / (2 * M_PI * (wheel / 2))) {

    leftMotors->set_encoder_units_all(pros::E_MOTOR_ENCODER_ROTATIONS);
    rightMotors->set_encoder_units_all(pros::E_MOTOR_ENCODER_ROTATIONS);
    state = DriveState::IDLE;
  }

  // tracking
  void startOdom(Odom *odom);

  // driver functions
  int inputCurve(int input, double t = 1);
  void arcadeMod(double forward, double turn, std::vector<double> curves, int speedThreshold, int speedCap);
  void arcade(double forward, double turn, std::vector<double> curves = {0, 0});
  void tank(double left, double right, std::vector<double> curves = {0, 0});

  // 1d pd movements

  /*
   * absolute turning
   * @param target: target angle in degrees
   * @param timeout: time in milliseconds to stop the movement
   * @param constants: PDconstants struct with kP and kD values
   */
  void turn(double target, PID pid, int maxSpeed = 127, double timeout = 800, bool async = false);

  /*
   * relative linear movement
   * @param target: target distance in inches
   * @param timeout: time in milliseconds to stop the movement
   * @param constants: PDconstants struct with kP and kD values
   */
  //void move(double target, PID pid, int maxSpeed = 127, double slewRate = 127, double timeout = 1000, bool async = true);
  void move(float target, PID linearPid, PID headingPid, float maxSpeed);

  /*
   * swing turn
   * @param target: target angle in degrees
   * @param speeds: vector of left max speed and right max speed, will turn in
   * direction with higher speed
   * @param timeout: time in milliseconds to stop the movement
   * @param constants: PDconstants struct with kP and kD values
   */
  void swing(double target, std::vector<int> speeds, double timeout,
             bool async = false);

  // 2d movements

  /*
   * boomerang
   * @param path: asset struct with path data
   * @param lookahead: distance in inches to look ahead
   * @param timeout: time in milliseconds to stop the movement
   */
  void boomerang(double x, double y, double theta, int timeout = 2000,
                 double dLead = 0.6, double gLead = 0.3, bool async = false,
                 double exitRange = .5);
};
} // namespace lib