#include "lib/chassis.h"
#include "lib/util.h"
#include "pros/llemu.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <cstdint>
#include <vector>



using namespace lib;


/*
void Chassis::move(double target, PID pid, int maxSpeed, double slewRate, double timeout, bool async) {

  if (async) {
    while (this->getState() == DriveState::MOVING) {
      pros::delay(20);
    }
    pros::Task task([&]() { move(target, pid, maxSpeed, slewRate, timeout, false); });
  }


  state = DriveState::MOVING;

  int startPos = UINT32_MAX;

  leftMotors->set_encoder_units_all(pros::E_MOTOR_ENCODER_ROTATIONS);
  rightMotors->set_encoder_units_all(pros::E_MOTOR_ENCODER_ROTATIONS);

  uint16_t now = pros::millis();

  float prevOutput = 0;


  while (true) {
    // get distance traveled by each motor
    std::vector<pros::MotorGears> gearsets = leftMotors->get_gearing_all();
    std::vector<double> positions = leftMotors->get_position_all();
    std::vector<float> distances;

    for (int i = 0; i < positions.size(); i++) {
      float gear;
      switch (gearsets[i]) {
      case pros::MotorGears::red:
        gear = 100;
        break;
      case pros::MotorGears::green:
        gear = 200;
        break;
      case pros::MotorGears::blue:
        gear = 600;
        break;
      default:
        gear = 200;
        break;
      }
      distances.push_back(positions[i] * (wheel * M_PI) * (rpm / gear));
      pros::lcd::print(2, "distances: %f", distances[i]);
    }

    if (startPos == UINT32_MAX) {
      startPos = avg(distances);
    }

    // calculate error
    double error = target + startPos - avg(distances);

    // return if we reach timout
    if (pros::millis() - now > timeout) {
      leftMotors->move(0);
      rightMotors->move(0);
      leftMotors->brake();
      rightMotors->brake();
      pid.reset();
      state = DriveState::IDLE;
      return;
    }
    // return if we reach target
    if (std::abs(error) <= .5 && std::abs(leftMotors->get_actual_velocity_all()[0]) < 5 && std::abs(rightMotors->get_actual_velocity_all()[0]) < 5) {
      leftMotors->move(0);
      rightMotors->move(0);
      pid.reset();
      state = DriveState::IDLE;
      return;
    }

    float output = slew(prevOutput, pid.update(error), slewRate);

    // clamp output
    if (output > maxSpeed) {
      output = maxSpeed;
    } else if (output < -maxSpeed) {
      output = -maxSpeed;
    }

    leftMotors->move(output);
    rightMotors->move(output);

    prevOutput = output;

    pros::lcd::print(1, "error: %f", error);
    pros::delay(15);
  }
}
*/


void Chassis::move(float target, float speedMultiplier, float maxSpeed) {
  float lStart;
  float rStart;

  state = DriveState::MOVING;


  while (true) {
    std::vector<double> lPositions = leftMotors->get_position_all();
    std::vector<float> lDistances;
    for (int i = 0; i < lPositions.size(); i++) {
      lDistances.push_back(lPositions[i] * (wheel * M_PI) * (rpm / 600.0)); // assumes blue gear cartridges
    }
    std::vector<double> rPositions = rightMotors->get_position_all();
    std::vector<float> rDistances;
    for (int i = 0; i< rPositions.size(); i++) {
      rDistances.push_back(rPositions[i] * (wheel * M_PI) * (rpm / 600.0));
    }

    if (lStart == 0) {
      lStart = avg(lDistances);
      rStart = avg(rDistances);
    }


    double lError = lStart + target - avg(lDistances);
    double rError = rStart + target - avg(rDistances);

    pros::lcd::print(1, "error: %f", lError);

    if (std::abs(lError) < 0.5 && std::abs(rError) < 0.5 && std::abs(leftMotors->get_actual_velocity()) < 5 && std::abs(rightMotors->get_actual_velocity()) < 5) {
      leftMotors->move(0);
      rightMotors->move(0);
      state = DriveState::IDLE;
      return;
    }
    
    leftMotors->move(fmin(lError * speedMultiplier, maxSpeed));
    rightMotors->move(fmin(rError * speedMultiplier, maxSpeed));
  }


}