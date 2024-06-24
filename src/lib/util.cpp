#include "lib/util.h"

float slew(float current, float target, float rate) {
  if (target - current > rate) {
    if (current < target) {
      current += rate;
    } else if (current > target) {
      current -= rate;
    }
    return current;
  } else {
    return target;
  }
}

float avg(std::vector<float> values) {
  float sum = 0;
  for (int i = 0; i < values.size(); i++) {
    sum += values[i];
  }
  return sum / values.size();
}