#pragma once


    #include <climits>
class PID {
    public:

        PID(float kP, float kI, float kD, float slewRate = INT_MAX);

        float update(float error);

        void reset();

    protected:
        
        const float kP;
        const float kI;
        const float kD;
        const float slewRate;

        float integral = 0;
        float prevError = 0;
        
        float prevOutput = 0;
        float output = 0;
    };