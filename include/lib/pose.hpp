namespace lib {

class Pose {
public:
  double x;
  double y;
  double theta;
  Pose(double x, double y, double theta) : x(x), y(y), theta(theta) {}
};
} // namespace lib