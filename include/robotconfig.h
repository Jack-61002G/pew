#pragma once
#include "main.h"
#include "lib/chassis.h"
#include "lib/pid.h"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"

pros::MotorGroup leftMotors({-20,-18,19}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

pros::MotorGroup rightMotors({11,14,-13}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

pros::Imu imu(15);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

lib::Chassis chassis = lib::Chassis(&leftMotors, &rightMotors, &imu, 450, 2.75);
pros::Motor intake(1);

PID linear(15,0,5);