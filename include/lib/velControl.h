#pragma once

namespace lib{

class velController{
    private:
        double kV, kP_Pos, kP_Vel;
        double currPos, currVel, power;

    public:

        velController(double ikV, double ikP_Pos, double ikP_Vel) : kV(ikV), kP_Pos(ikP_Pos), kP_Vel(ikP_Vel) {};

        double step(double position, double velocity, double currentPos, double currentVel);

        double getTargetPower() const;
};

}
