#pragma once
#include "main.h"
#include "lib/chassis.h"
#include "lib/pid.h"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"

pros::MotorGroup leftMotors({-20,18,-19}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

pros::MotorGroup rightMotors({21,7,-9}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

pros::Imu imu(17);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

lib::Chassis chassis = lib::Chassis(&leftMotors, &rightMotors, &imu, 450, 2.75);
pros::Motor intake(1);

PID linear(11,0,6, 45);
PID heading(0.2, 0.0001, 0.15);