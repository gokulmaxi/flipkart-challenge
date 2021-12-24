#include <vector>
enum Axis
{
  x,
  y
};
class Goal
{
public:
  Axis axis;
  float point;
  Goal()
  {
    axis = x;
    point = 0;
  }
  Goal(Axis _axis, float _point)
  {
    axis = _axis;
    point = _point;
  }
};
//                                      1       , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14
//  use this const double xPoint[14]                 = {0.29 , 0.49 , 0.67 , 0.86 , 1.06 , 1.25 , 1.45 , 1.63 , 1.83  , 2.02  , 2.2  , 2.4  , 2.59, 2.78};
const double xPoint[14] = {0.45, 0.64, 0.83, 1.02, 1.22, 1.42, 1.61,1.78, 1.98, 2.18, 2.36, 2.55, 2.74, 2.94};
const double yPoint[14] = {0.20, 0.38, 0.58, 0.77, 0.96, 1.15, 1.34, 1.55, 1.74, 1.94, 2.10, 2.3, 2.50, 2.68};
//waypoints from induct one to goals
std::vector<Goal> one_one_waypoint = {Goal(x, 3)};
std::vector<Goal> one_two_waypoint = {Goal(x, 7)};
std::vector<Goal> one_three_waypoint = {Goal(x, 11)};
std::vector<Goal> one_four_waypoint = {Goal(y, 6), Goal(x, 3)};
std::vector<Goal> one_five_waypoint = {Goal(y, 6), Goal(x, 7)};
std::vector<Goal> one_six_waypoint = {Goal(y, 6), Goal(x, 11)};
std::vector<Goal> one_seven_waypoint = {Goal(y, 6), Goal(x, 11)};
std::vector<Goal> one_eight_waypoint = {Goal(y, 6), Goal(x, 11)};
std::vector<Goal> one_nine_waypoint = {Goal(y, 6), Goal(x, 11)};
//waypoints from induct two to goals
std::vector<Goal> two_one_waypoint = {Goal(x, 3)};
std::vector<Goal> two_two_waypoint = {Goal(x, 7)};
std::vector<Goal> two_three_waypoint = {Goal(x, 11)};
std::vector<Goal> two_four_waypoint = {Goal(x, 3)};
std::vector<Goal> two_five_waypoint = {Goal(x, 7)};
std::vector<Goal> two_six_waypoint = {Goal(x, 11)};
std::vector<Goal> two_seven_waypoint = {Goal(x, 11)};
std::vector<Goal> two_eight_waypoint = {Goal(x, 11)};
std::vector<Goal> two_nine_waypoint = {Goal(x, 11)};
