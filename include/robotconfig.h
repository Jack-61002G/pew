#pragma once
#include "lib/lift.hpp"
#include "lib/chassis.h"
#include "lib/pid.h"
#include "pros/adi.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"

//pros::Motor armMotor(20);
//lib::Lift arm(armMotor, 12.0/84, {1, 0.0001, 1});

inline pros::MotorGroup leftMotors({-9,7,-10}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

inline pros::MotorGroup rightMotors({1,-2,3}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

inline pros::Imu imu(8);

inline pros::Rotation rot(6);
inline lib::TrackingWheel track(rot, 2.125);

inline pros::Controller controller(pros::E_CONTROLLER_MASTER);

inline lib::Chassis chassis = lib::Chassis(&leftMotors, &rightMotors, &imu, &track, 450, 2.75);

inline pros::MotorGroup intake({-18, -17});

inline pros::MotorGroup armMotors({5});
inline pros::adi::Pneumatics wrist('H', false);
inline lib::Lift lift(&armMotors, &wrist, 1.0/7.0, {2, 0.1, 2.5});

inline pros::adi::Pneumatics pisstake('F', false);
inline pros::adi::Pneumatics clamp('E', false);
inline pros::adi::Pneumatics claw('G', false);

inline PID linear(10,0.0, 34, 45);
inline PID heading(1.6, 0.0052, 15);
inline PID turning(3.75, 0.0025, 37);
inline PID movetoturn(0.35, .00015, 9);

//inline pros::adi::Led leftArmLed('C', 48);
//inline pros::adi::Led rightArmLed('D', 48);
inline pros::adi::Led leftDriveLed('B', 48);
inline pros::adi::Led rightDriveLed('A', 48);