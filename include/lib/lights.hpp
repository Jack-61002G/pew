#include "pros/adi.hpp"
#include "lib/TaskWrapper.hpp"
#include <cstdint>


namespace lib {


class Lights : public ryan::TaskWrapper {

private:
    pros::adi::Led leftDriveLed = pros::adi::Led('E', 40);
    pros::adi::Led rightDriveLed = pros::adi::Led('G', 40);
    pros::adi::Led indicatorLed = pros::adi::Led('C', 40);

    const uint32_t red = 0x850000;
    const uint32_t blue = 0x000080;
    const uint32_t urple = 0x620062;

    const uint32_t ind_on = 0xA0A0A0;
    const uint32_t ind_off = 0x000000;

    const uint32_t warning_30 = 0x00FF00;
    const uint32_t warning_15 = 0x858500;

public:
    int team = 0;
    int indicator = 0;
    int startTime = -1;

    void startTimer() { startTime = pros::millis() - 75000; }
    void stopTimer() { startTime = -1; }
    void loop() override;

};


}