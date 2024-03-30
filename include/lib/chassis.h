#pragma once

#include "lib/diffy.h"
#include "lib/pd.h"
#include "lib/profiler.hpp"
#include <utility>
#include <vector>


namespace lib {

class Chassis {

    private:
        Diffy* motors;
        
    public:

        //driver functions
        int inputCurve(int input, double t = 1);
        void arcadeMod(double forward, double turn, std::vector<double> curves, int speedThreshold, int speedCap);
        void arcade(double forward, double turn, std::vector<double> curves = {0, 0});
        void tank(double left, double right, std::vector<double> curves = {0, 0});
        
        //motion profiling
        void moveProfiled(double target, profileConstraints constraints);

        // 1d pd movements

        /*
        * absolute turning
        * @param target: target angle in degrees
        * @param timeout: time in milliseconds to stop the movement
        * @param constants: PDconstants struct with kP and kD values
        */
        void pdTurn(double target, int maxSpeed, double timeout, PDconstants constants);

        /*
        * relative linear movement
        * @param target: target distance in inches
        * @param timeout: time in milliseconds to stop the movement
        * @param constants: PDconstants struct with kP and kD values
        */
        void pdMove(double target, int maxSpeed, double timeout, PDconstants constants);

        /*
        * swing turn
        * @param target: target angle in degrees
        * @param speeds: vector of left max speed and right max speed, will turn in direction with higher speed
        * @param timeout: time in milliseconds to stop the movement
        * @param constants: PDconstants struct with kP and kD values
        */
        void swing(double target, std::vector<int> speeds, double timeout, PDconstants constants);

        // 2d pd movements
        
};

} // namespace lib