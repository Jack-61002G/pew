#pragma once
#include "main.h"
#include "lib/chassis.h"
#include "lib/pid.h"
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rotation.hpp"

pros::MotorGroup leftMotors({9,-8,-10}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

pros::MotorGroup rightMotors({1,-2,3}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

pros::Imu imu(7);

pros::Rotation rot(4);
lib::TrackingWheel track(rot, 2);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

lib::Chassis chassis = lib::Chassis(&leftMotors, &rightMotors, &imu, &track, 450, 2.75);

pros::Motor intake(-20);

pros::adi::Pneumatics pisstake('H', false);
pros::adi::Pneumatics clamp('G', false);

PID linear(11,0,6, 45);
PID heading(0.2, 0.0001, 0.15);
PID turning(2.5, 0.005, 15);

