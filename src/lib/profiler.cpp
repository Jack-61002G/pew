#include <vector>
#include <cmath>
#include "lib/profiler.hpp"

using namespace lib;

std::vector<std::pair<double, double>> Profiler::generateProfile(double target_position) {
    std::vector<std::pair<double, double>> profile;
    double time_step = 0.015; // Time step in seconds (10 ms)

    // Calculate the time it takes to accelerate to max velocity
    double acceleration_dt = max_vel / max_accel;

    // If we can't accelerate to max velocity in the given distance, we'll accelerate as much as possible
    double halfway_distance = target_position / 2;
    double acceleration_distance = 0.5 * max_accel * pow(acceleration_dt, 2);

    if (acceleration_distance > halfway_distance) {
        acceleration_dt = sqrt(halfway_distance / (0.5 * max_accel));
    }

    acceleration_distance = 0.5 * max_accel * pow(acceleration_dt, 2);

    // Recalculate max velocity based on the time we have to accelerate and decelerate
    max_vel = max_accel * acceleration_dt;

    // We decelerate at a different rate than we accelerate
    double deceleration_dt = max_vel / max_decel;

    // Calculate the time that we're at max velocity
    double cruise_distance = target_position - acceleration_distance - 0.5 * max_decel * pow(deceleration_dt, 2);
    double cruise_dt = cruise_distance / max_vel;
    double deceleration_time = acceleration_dt + cruise_dt;

    // Check if we're still in the motion profile
    double entire_dt = acceleration_dt + cruise_dt + deceleration_dt;

    double position = 0.0;
    double velocity = 0.0;

    for (double elapsed_time = 0; elapsed_time <= entire_dt; elapsed_time += time_step) {

        // If we're accelerating
        if (elapsed_time < acceleration_dt) {
            velocity += max_accel * time_step;
            if (velocity > max_vel) {
                velocity = max_vel;
            }
        }
        // If we're decelerating
        else if (elapsed_time >= deceleration_time) {
            velocity -= max_decel * time_step;
            if (velocity < 0) {
                velocity = 0;
            }
        }

        position += velocity * time_step;
        profile.push_back({velocity, position});
    }

    return profile;
}