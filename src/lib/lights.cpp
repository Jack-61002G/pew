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

        if (40000 < dT && dT < 40500) { // 30 second warning
        if (currentWarning != 1) {
            currentWarning = 1;
            leftDriveLed.set_all(warning_30);
            pros::delay(20);
            rightDriveLed.set_all(warning_30);
            pros::delay(20);
        }}
        else if (50000 < dT && dT < 50500) { // 15 second warning
        if (currentWarning != 2) {
            currentWarning = 2;
            leftDriveLed.set_all(warning_15);
            pros::delay(20);
            rightDriveLed.set_all(warning_15);
            pros::delay(20);
        }}
        else if (55000 < dT && dT < 55500) { // 0 second warning
        if (currentWarning != 3) {
            currentWarning = 3;
            leftDriveLed.set_all(warning_0);
            pros::delay(20);
            rightDriveLed.set_all(warning_0);
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

        // update indicator lights
        if (indicator != currentIndicator) {
            currentIndicator = indicator;
            indicatorLed1.set_all(indicator == 1 ? ind_on : ind_off);
            indicatorLed2.set_all(indicator == 1 ? ind_on : ind_off);
            pros::delay(20);
        }

        pros::delay(20);

    }
}