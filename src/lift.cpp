#include "lift.hpp"
#include <cstdint>

void Lift::setLift(double angle) {
  if (this->isMoving) {
    pros::delay(15);
  }
  calculating = true;
  this->profile = this->profiler->generateProfile((angle / 6) / gearRatio);
  this->angleTarget = angle / gearRatio;
  calculating = false;
  holding = false;
}

void Lift::loop() {

  while (true) {

    double prevError = 0;
    double i_out = 0;

    std::uint32_t now = pros::millis();

    for (std::pair<double, double> point : profile) {

      // calculate feedforward value based on error between target velocity and
      // current velocity
      double ff_out = this->ff * (point.first - liftMotor->getDiffyVel()[0]);

      // pid calculations for distance error
      double error = point.second - liftMotor->getDiffyPos()[0];
      double p_out = this->kP * error;
      i_out += this->kI * error;
      double d_out = this->kD * (error - prevError);

      // set previous error to current error
      prevError = error;

      // anti windup
      if (i_out > 127) {
        i_out = 127;
      } else if (i_out < -127) {
        i_out = -127;
      }
      // reset i when sign changes
      if (error * prevError < 0) {
        i_out = 0;
      }

      // combine ff and pid output
      double output = ff_out + p_out + i_out + d_out;

      // set motor voltage

      liftMotor->spinDiffy(output);

      pros::Task::delay_until(&now, 15);
    }

    isMoving = false;
    holding = true;

    //hold the last position
    std::pair<double,double> point = profile.back();
    while (holding) {

        
        // calculate feedforward value based on error between target velocity
        // and current velocity
        double ff_out = this->ff * (point.first - liftMotor->getDiffyVel()[0]);

        // pid calculations for distance error
        double error = point.second - liftMotor->getDiffyPos()[0];
        double p_out = this->kP * error;
        i_out += this->kI * error;
        double d_out = this->kD * (error - prevError);

        // set previous error to current error
        prevError = error;

        // anti windup
        if (i_out > 127) {
          i_out = 127;
        } else if (i_out < -127) {
          i_out = -127;
        }
        // reset i when sign changes
        if (error * prevError < 0) {
          i_out = 0;
        }

        // combine ff and pid output
        double output = ff_out + p_out + i_out + d_out;

        // set motor voltage

        liftMotor->spinDiffy(output);

        pros::Task::delay_until(&now, 15);
      }
    }
  }