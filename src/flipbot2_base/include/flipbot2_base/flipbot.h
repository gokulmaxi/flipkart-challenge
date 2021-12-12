#include "math.h"
#include <cmath>
enum Axis { x, y };
class Goal {
public:
  Axis axis;
  float point;
  Goal(Axis _axis, float _point) {
    axis = _axis;
    point = _point;
  }
};
/**
 * @brief: calculate the euclidean distance
 *
 * @param: int startX
 *       : int endX
 *
 * @return: double
 */
double euclidianDistance(double start, double end) {
  double distance = 0.0;
  distance = std::sqrt(pow((start - end), 2));
  return distance;
}
