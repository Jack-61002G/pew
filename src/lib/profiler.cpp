#include <vector>
#include <cmath>
#include "lib/profiler.hpp"

namespace lib {

double max_velocity = 61;
double max_acceleration = 120;
double max_deceleration = 120;

std::vector<std::pair<double, double>> Profiler::generateProfile(double target_position, double max_velocity, double max_acceleration ,double max_deceleration) {
    std::vector<std::pair<double, double>> profile;
    double time_step = 0.01; // Time step in seconds (10 ms)

    // Calculate the time it takes to accelerate to max velocity
    double acceleration_dt = max_velocity / max_acceleration;

    // If we can't accelerate to max velocity in the given distance, we'll accelerate as much as possible
    double halfway_distance = target_position / 2;
    double acceleration_distance = 0.5 * max_acceleration * pow(acceleration_dt, 2);

    if (acceleration_distance > halfway_distance) {
        acceleration_dt = sqrt(halfway_distance / (0.5 * max_acceleration));
    }

    acceleration_distance = 0.5 * max_acceleration * pow(acceleration_dt, 2);

    // Recalculate max velocity based on the time we have to accelerate and decelerate
    max_velocity = max_acceleration * acceleration_dt;

    // We decelerate at a different rate than we accelerate
    double deceleration_dt = max_velocity / max_deceleration;

    // Calculate the time that we're at max velocity
    double cruise_distance = target_position - acceleration_distance - 0.5 * max_deceleration * pow(deceleration_dt, 2);
    double cruise_dt = cruise_distance / max_velocity;
    double deceleration_time = acceleration_dt + cruise_dt;

    // Check if we're still in the motion profile
    double entire_dt = acceleration_dt + cruise_dt + deceleration_dt;

    double position = 0.0;
    double velocity = 0.0;

    for (double elapsed_time = 0; elapsed_time <= entire_dt; elapsed_time += time_step) {

        // If we're accelerating
        if (elapsed_time < acceleration_dt) {
            velocity += max_acceleration * time_step;
            if (velocity > max_velocity) {
                velocity = max_velocity;
            }
        }
        // If we're decelerating
        else if (elapsed_time >= deceleration_time) {
            velocity -= max_deceleration * time_step;
            if (velocity < 0) {
                velocity = 0;
            }
        }

        position += velocity * time_step;
        profile.push_back({velocity, position});
    }

    return profile;
}

} // namespace lib