#pragma once


    class PID {
    public:

        PID(float kP, float kI, float kD);

        float update(float error);

        void reset();

    protected:
        
        const float kP;
        const float kI;
        const float kD;

        float integral = 0;
        float prevError = 0;
    };