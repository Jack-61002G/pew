#include <vector>

namespace lib {

class profileConstraints {
public:
  double max_velocity;
  double max_acceleration;
  double max_deceleration;

  profileConstraints(double max_velocity, double max_acceleration,
                     double max_deceleration) {
    this->max_velocity = max_velocity;
    this->max_acceleration = max_acceleration;
    this->max_deceleration = max_deceleration;
  }
};

class Profiler {
public:
  static double max_vel;
  static double max_accel;
  static double max_decel;

  std::vector<std::pair<double, double>>
  generateProfile(double target_position, double max_velocity = max_vel,
                  double max_acceleration = max_accel,
                  double max_deceleration = max_decel);

  Profiler(double max_velocity, double max_acceleration,
           double max_deceleration) {
    this->max_vel = max_velocity;
    this->max_accel = max_acceleration;
    this->max_decel = max_deceleration;
  }
};

} // namespace lib