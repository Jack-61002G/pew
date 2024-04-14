#include "tracers.h"
#include "pros/imu.hpp"
#include "lib/pose.hpp"
namespace lib {


class Odom {
private:
    std::pair<Tracer, Tracer> *tracers;
    pros::Imu *imu;
    Pose currentPose = Pose(0, 0, 0);
public:
    //constructor
    Odom(std::pair<Tracer, Tracer> *tracers, pros::Imu *imu) {
        this->tracers = tracers;
        this->imu = imu;
    }

    //tracking
    void startTracking();
    void getPose();
    void setPose();

};

} // namespace tracking