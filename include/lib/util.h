#pragma once

template <typename T> constexpr T sgn(T value) { return value < 0 ? -1 : 1; }