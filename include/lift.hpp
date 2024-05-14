#include "lib/diffy.h"
#include "lib/profiler.hpp"
#include "lib/TaskWrapper.hpp"
#include <utility>

class Lift : public ryan::TaskWrapper{
private:
  lib::Diffy *liftMotor;
  lib::Profiler *profiler;

  double ff;
  double kP;
  double kI;
  double kD;

  double gearRatio;

  bool calculating = false;
  bool isMoving = false;
  bool holding = false;


  double angleTarget;
  std::vector<std::pair<double, double>> profile;

public:
  Lift(std::vector<int> ports, lib::profileConstraints constraints, double ff,
       double kP, double kI, double kD, double gearRatio = 1) {
    liftMotor = new lib::Diffy(ports);
    profiler = new lib::Profiler(constraints);
  }

  void setLift(double angle);

  void loop() override;

};
