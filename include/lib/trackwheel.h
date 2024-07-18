#include "pros/rotation.hpp"
#include "lib/util.h"
namespace lib {



class TrackingWheel {

private:
    pros::Rotation rot;
    float wheel;

public:
    double getDistance() { return rot.get_position() / 36000.0 * wheel * M_PI; }

    TrackingWheel(pros::Rotation rot, float wheel) : rot(rot), wheel(wheel) {
        rot.reset_position();
    }
};
}