#pragma once
#include "lib/TaskWrapper.hpp"
#include "main.h"
#include <memory>


namespace lib {

class Lights : public ryan::TaskWrapper {

private:
    std::shared_ptr<pros::adi::Led> leftArmLed;
    std::shared_ptr<pros::adi::Led> rightArmLed;
    std::shared_ptr<pros::adi::Led> leftDriveLed;
    std::shared_ptr<pros::adi::Led> rightDriveLed;


public:
    Lights(pros::adi::Led *leftArmLed, pros::adi::Led *rightArmLed, pros::adi::Led *leftDriveLed, pros::adi::Led *rightDriveLed):
    leftArmLed(leftArmLed), rightArmLed(rightArmLed), leftDriveLed(leftDriveLed), rightDriveLed(rightDriveLed) {}
    void loop() override;

};
}