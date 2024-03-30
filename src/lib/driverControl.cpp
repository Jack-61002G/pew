#include "lib/chassis.h"
#include <cmath>

using namespace lib;

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

int Chassis::inputCurve(int input, double t) {
  return std::pow(input, t) / std::pow(127, t - 1);
}

void Chassis::arcade(double forward, double turn, std::vector<double> curves) {
  // curve input
  forward = inputCurve(forward, curves[0]);
  turn = inputCurve(turn, curves[1]);

  // arcade drive
  motors->spinDiffy(forward + turn, forward - turn);
}

void Chassis::tank(double left, double right, std::vector<double> curves) {
  // curve input
  left = inputCurve(left, curves[0]);
  right = inputCurve(right, curves[1]);

  // tank drive
  motors->spinDiffy(left, right);
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
}