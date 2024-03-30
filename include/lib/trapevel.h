#pragma once
#ifndef TRAPEVEL_H
#define TRAPEVEL_H

class trapevel {

private:
  double k_UP, k_DOWN, ff, kP;
  double prevVelocity = 0;

public:
  /*
   * @brief Construct a new trapevel
   *
   * @param k_UP
   * @param k_DOWN
   * @param ff
   * @param kp
   *
   * Tuning Guide:
   * 1. increase ff until top speed matches the cruising speed (do not
   * overshoot)
   * 2. adjust the down until it bounces at the end then reduce until good
   * (overshoot is fine here)
   * 3. adjust up until overshoots then reduce until good (undershoot is fine
   * here)
   * 4. adjust kp until most error is gone or movements are smooth and
   * consistent
   */
  trapevel(double k_UP = 0, double k_DOWN = 0, double ff = 0, double kp = 0);

  double update(double velocity, double actual);

  void reset();
};

#endif