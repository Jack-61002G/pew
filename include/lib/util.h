#pragma once
#include "main.h"

template <typename T> constexpr T sgn(T value) { return value < 0 ? -1 : 1; }

float slew(float current, float target, float rate);

float avg(std::vector<float> values);