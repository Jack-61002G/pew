#pragma once
#include "main.h"
#include "lib/chassis.h"
#include "pros/motors.hpp"

pros::MotorGroup leftMotors({-20,-18,19});

pros::MotorGroup rightMotors({11,14,-13});

pros::Imu imu(15);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

lib::Chassis chassis = lib::Chassis(&leftMotors, &rightMotors, &imu, 450, 2.75);
pros::Motor intake(1);
