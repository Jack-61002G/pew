#include "lights.hpp"
#include <cstdint>
#include <exception>



void lib::Lights::loop() {
    int currentTeam = -1; // skills, red, blue
    int currentIndicator = -1;
    int currentWarning = 0;

    while (true) {
        

        // update time warning lights
        if (startTime != -1) {
        int dT = pros::millis() - startTime;

        if (85000 < dT && dT < 85500) { // 30 second warning
        if (currentWarning != 1) {
            currentWarning = 1;
            leftDriveLed.set_all(warning_30);
            pros::delay(20);
            rightDriveLed.set_all(warning_30);
            pros::delay(20);
        }}
        else if (90000 < dT && dT < 90500) { // 15 second warning
        if (currentWarning != 2) {
            currentWarning = 2;
            leftDriveLed.set_all(warning_15);
            pros::delay(20);
            rightDriveLed.set_all(warning_15);
            pros::delay(20);
        }}
        else if (currentWarning != 0) { // clearing warning
            currentWarning = 0;
            currentTeam = team;
            leftDriveLed.set_all(team == 1 ? red : team == 2 ? blue : urple);
            pros::delay(20);
            rightDriveLed.set_all(team == 1 ? red : team == 2 ? blue : urple);
            pros::delay(20);
        }
        }

        // update team color lights
        if (team != currentTeam && currentWarning == 0) {
            currentTeam = team;
            leftDriveLed.set_all(team == 1 ? red : team == 2 ? blue : urple);
            pros::delay(20);
            rightDriveLed.set_all(team == 1 ? red : team == 2 ? blue : urple);
            pros::delay(20);
        }

        // update indicator light
        if (indicator != currentIndicator) {
            currentIndicator = indicator;
            indicatorLed.set_all(indicator == 1 ? ind_on : ind_off);
            pros::delay(20);
        }

        pros::delay(20);

    }
}