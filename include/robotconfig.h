#pragma once
#include "main.h"
#include "lib/chassis.h"

pros::MotorGroup leftMotors({1,2,3});

pros::MotorGroup rightMotors({4,5,6});

pros::Imu imu(7);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

lib::Chassis chassis = lib::Chassis(&leftMotors, &rightMotors, &imu, 450, 2.75);
