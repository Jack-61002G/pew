#include "tracers.h"
#include "pros/imu.hpp"
#include "lib/point.hpp"
namespace lib {


class Odom {
private:
    std::pair<Tracer, Tracer> *tracers;
    pros::Imu *imu;
    Point currentPose = Point(0, 0, 0);
public:
    //constructor
    Odom(std::pair<Tracer, Tracer> *tracers, pros::Imu *imu) {
        this->tracers = tracers;
        this->imu = imu;
    }

    //tracking
    void startTracking();
    Point getPose();
    void setPose();

};

} // namespace tracking