#pragma once
#include "lib/lift.hpp"
#include "lib/chassis.h"
#include "lib/pid.h"
#include "lib/lights.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "robotconfig.h"


inline pros::MotorGroup leftMotors({-16,-13,14}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

inline pros::MotorGroup rightMotors({11,-19,20}, pros::v5::MotorGears::blue, pros::v5::MotorUnits::rotations);

inline pros::Imu imu(8);

inline pros::Rotation rot(6);
inline lib::TrackingWheel track(rot, 2.125);

inline pros::Controller controller(pros::E_CONTROLLER_MASTER);

inline lib::Chassis chassis = lib::Chassis(&leftMotors, &rightMotors, &imu, &track, 450, 2.75);

inline pros::MotorGroup intake({7});

inline pros::MotorGroup armMotors({6, -8});
inline lib::Lift lift(&armMotors,  36.0/60, {2.5, 0, 3.75});

inline pros::adi::Pneumatics doinker('H', false);
inline pros::adi::Pneumatics clamp('D', false);
inline pros::adi::Pneumatics sorter('F', false);

inline PID linear(10,0.0, 34);
inline PID heading(2.875, 0, 20, 0);
inline PID turning(2.875, 0.003, 20, 15);
inline PID swing(.5, 0.0, 25, 7.5);

inline lib::Lights lights = lib::Lights();

inline rd::Console console;