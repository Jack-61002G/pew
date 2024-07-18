#pragma once
#include "lib/point.hpp"
#include "lib/trackwheel.h"
#include "pros/imu.hpp"
#include <memory>

namespace lib {

class Odom {
private:
  Point currentPose = Point(0, 0, 0);
  std::shared_ptr<TrackingWheel> vertiTracker;
  std::shared_ptr<pros::Imu> imu;

public:
  // constructor
  Odom(TrackingWheel vertiTracker, pros::Imu imu) {
    this->vertiTracker = std::make_shared<TrackingWheel>(vertiTracker);
    this->imu = std::make_shared<pros::Imu>(imu);
  };

  // tracking
  void startTracking();
  Point getPose(bool radians = false);
  void setPose(Point newPose);
};

} // namespace lib