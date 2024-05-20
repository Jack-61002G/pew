#include "tracers.h"
#include "pros/imu.hpp"
#include "lib/point.hpp"
namespace lib {


class Odom {
private:
    std::pair<Tracer, Tracer> *tracers;
    Point currentPose = Point(0, 0, 0);
public:
    //constructor
    Odom(std::pair<Tracer, Tracer> *tracers) : tracers(tracers) {}

    //tracking
    void startTracking();
    Point getPose(bool radians = false);
    void setPose();

};

} // namespace tracking