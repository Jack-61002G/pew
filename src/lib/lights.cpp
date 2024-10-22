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

        if (5000 < dT && dT < 5500) { // 20 second warning
        if (currentWarning != 1) {
            currentWarning = 1;
            leftDriveLed.set_all(warning_1);
            pros::delay(20);
            rightDriveLed.set_all(warning_1);
            pros::delay(20);
        }}
        else if (10000 < dT && dT < 10500) { // 10 second warning
        if (currentWarning != 2) {
            currentWarning = 2;
            leftDriveLed.set_all(warning_2);
            pros::delay(20);
            rightDriveLed.set_all(warning_2);
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
            if (indicator == 1) {
                indicatorLed1.set_all(team == 1 ? red : blue);
                pros::delay(20);
                indicatorLed2.set_all(team == 1 ? red : blue);
            } else {
                indicatorLed1.clear();
                pros::delay(20);
                indicatorLed2.clear();
            }
        }

        pros::delay(20);

    }
}