#include "pros/imu.hpp"
#include "lib/point.hpp"
namespace lib {


class Odom {
private:
    Point currentPose = Point(0, 0, 0);
public:
    //constructor
    Odom();

    //tracking
    void startTracking();
    Point getPose(bool radians = false);
    void setPose();

};

} // namespace tracking