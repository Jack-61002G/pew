#pragma once
#include "main.h"

template <typename T> constexpr T sgn(T value) { return value < 0 ? -1 : 1; }

float slew(float current, float target, float rate);

float avg(std::vector<float> values);

float constrain180(float input);

float constrain90(float input);

float relative90(float input, float current);

float degreesToRadians(float degrees);

float radiansToDegrees(float radians);

float angleError(float current, float target);