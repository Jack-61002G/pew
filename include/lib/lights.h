#pragma once
#include "lib/TaskWrapper.hpp"
#include "main.h"


namespace lib {

class Lights : public ryan::TaskWrapper {

private:
    std::shared_ptr<pros::adi::Led> leftArmLed;
    std::shared_ptr<pros::adi::Led> rightArmLed;


public:
    Lights(pros::adi::Led *leftArmLed, pros::adi::Led *rightArmLed) : leftArmLed(leftArmLed), rightArmLed(rightArmLed) {}

    void loop() override;

};
}