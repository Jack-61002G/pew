#include "lib/chassis.h"
#include <cmath>

using namespace lib;

int Chassis::inputCurve(int input, double t) {
  return std::pow(input, t) / std::pow(127 * (input / std::abs(input)), t - 1);
}

void Chassis::arcade(double forward, double turn, std::vector<double> curves) {
  // curve input
  forward = inputCurve(forward, curves[0]);
  turn = inputCurve(turn, curves[1]);
}
void Chassis::arcade(double forward, double turn){
  // arcade drive
  leftMotors->move(forward + turn);
  rightMotors->move(forward - turn);
}

void Chassis::tank(double left, double right, std::vector<double> curves) {
  // curve input
  left = inputCurve(left, curves[0]);
  right = inputCurve(right, curves[1]);

  // tank drive
  leftMotors->move(left);
  rightMotors->move(right);
}

void Chassis::arcadeMod(double forward, double turn, std::vector<double> curves,
                        int speedThreshold, int speedCap) {

  // curve input
  forward = inputCurve(forward, curves[0]);
  turn = inputCurve(turn, curves[1]);

  if (std::abs(forward) > speedThreshold && std::abs(turn) > speedThreshold) {
    if (forward < 0) {
      forward = -speedCap;
    } else {
      forward = speedCap;
    }
  }

  // arcade drive
  leftMotors->move(forward + turn);
  rightMotors->move(forward - turn);
}