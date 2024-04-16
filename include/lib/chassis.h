#pragma once

#include "asset.hpp"
#include "lib/diffy.h"
#include "lib/pd.h"
#include "lib/profiler.hpp"
#include "pros/imu.hpp"
#include "tracers.h"
#include <utility>
#include <vector>
#include "lib/odom.hpp"

namespace lib {

class Chassis {

private:
  enum class DriveState { IDLE, MOVING };
  Diffy *motors;
  pros::Imu *imu;
  DriveState state;
  std::pair<Tracer, Tracer> *tracers = nullptr;
  int headingTarget;
  bool correctHeading{false};

public:

  Odom *odom = nullptr;

  DriveState getState() { return state; }

  // constructors
  Chassis(Diffy *motors, pros::Imu *imu) {
    this->motors = motors;
    this->imu = imu;
    this->state = DriveState::IDLE;
  }

  Chassis(std::vector<int> ports, int imu) {
    this->motors = new Diffy(ports);
    this->imu = new pros::Imu(imu);
    this->state = DriveState::IDLE;
  }

//tracking
  void addOdom(std::pair<Tracer, Tracer> *tracers);
  void startOdom();

  // driver functions
  int inputCurve(int input, double t = 1);
  void arcadeMod(double forward, double turn, std::vector<double> curves,
                 int speedThreshold, int speedCap);
  void arcade(double forward, double turn, std::vector<double> curves = {0, 0});
  void tank(double left, double right, std::vector<double> curves = {0, 0});

  // motion profiling
  void moveProfiled(double target, profileConstraints constraints,
                    bool async = false);

  // 1d pd movements

  /*
  * heading correction task 
  * @param constants: PDconstants struct with kP and kD values
  */
  void headingTask(PDconstants constants);

  /*
   * absolute turning
   * @param target: target angle in degrees
   * @param timeout: time in milliseconds to stop the movement
   * @param constants: PDconstants struct with kP and kD values
   */
  void pdTurn(double target, int maxSpeed, double timeout,
              PDconstants constants, bool async = false);

  /*
   * relative linear movement
   * @param target: target distance in inches
   * @param timeout: time in milliseconds to stop the movement
   * @param constants: PDconstants struct with kP and kD values
   */
  void pdMove(double target, int maxSpeed, double timeout,
              PDconstants constants, bool async = false);

  /*
   * swing turn
   * @param target: target angle in degrees
   * @param speeds: vector of left max speed and right max speed, will turn in
   * direction with higher speed
   * @param timeout: time in milliseconds to stop the movement
   * @param constants: PDconstants struct with kP and kD values
   */
  void swing(double target, std::vector<int> speeds, double timeout,
             PDconstants constants, bool async = false);

  // 2d movements

  /*
   * pure pursuit
   * @param path: asset struct with path data
   * @param lookahead: distance in inches to look ahead
   * @param timeout: time in milliseconds to stop the movement
   */
  void purePursuit(asset path, double lookahead, double timeout);
};

} // namespace lib