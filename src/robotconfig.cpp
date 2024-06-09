#include "robotconfig.h"

pros::Motor leftFront(1);
pros::Motor leftMid(2);
pros::Motor leftBack(3);
pros::Motorgroup leftMotors({leftFront, leftMid, leftBack});

pros::Motor rightFront(4);
pros::Motor rightMid(5);
pros::Motor rightBack(6);
pros::Motorgroup rightMotors({rightFront, rightMid, rightBack});


pros::Imu imu(7);


lib::Chassis chassis = lib::Chassis(&leftMotors, &rightMotors, &imu, 450, 3.25);


