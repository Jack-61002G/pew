#pragma once
#include "lib/chassis.h"
#include "lib/lift.hpp"
#include "lib/lights.hpp"
#include "lib/pid.h"
#include "lib/trajectory.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "robotconfig.h"

inline pros::MotorGroup leftMotors({-11, -13, 14}, pros::v5::MotorGears::blue,
                                   pros::v5::MotorUnits::rotations);

inline pros::MotorGroup rightMotors({16, -17, 18}, pros::v5::MotorGears::blue,
                                    pros::v5::MotorUnits::rotations);

inline pros::Imu imu(19);

inline pros::Rotation rot(20);
inline lib::TrackingWheel track(rot, 2.125);

inline pros::Controller controller(pros::E_CONTROLLER_MASTER);

inline lib::Chassis chassis =
    lib::Chassis(&leftMotors, &rightMotors, &imu, &track, 450, 2.75);

inline pros::MotorGroup intake({7});

inline pros::MotorGroup armMotors({-6, 8});
inline lib::Lift lift(&armMotors, 12.0 / 60, {2.5, 0, 1.5});

inline pros::adi::Pneumatics doinker('H', false);
inline pros::adi::Pneumatics clamp('D', false);
inline pros::adi::Pneumatics sorter('F', false);

inline PID linear(10, 0.0, 34);
inline PID heading(2.875, 0, 20, 0);
inline PID turning(2.875, 0.003, 20, 15);
inline PID swing(3.2, 0.003, 22, 15);

inline lib::Lights lights = lib::Lights();

inline rd::Console console;

inline MotionProfileGenerator generator;
