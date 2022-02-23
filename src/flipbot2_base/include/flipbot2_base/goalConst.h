#include "unordered_map"
#include <vector>
enum Axis
{
  x,
  y,
  cx,
  cy,
  px,py
};
class Goal
{
public:
  Axis axis;
  int point;
  int checkPoint;
  Goal()
  {
    axis = x;
    point = 0;
    checkPoint = 0;
  }
  Goal(Axis _axis, float _point, int _checkPoint = 0)
  {
    axis = _axis;
    point = _point;
    checkPoint = _checkPoint;
  }
};
//                        { 1     , 2     , 3     , 4       , 5     , 6     , 7     , 8     , 9     , 10    , 11    , 12    , 13    , 14      , 15}
const double xPoint[16] = {0.755, 0.905, 1.057, 1.220, 1.373, 1.542, 1.688, 1.840, 1.995, 2.150, 2.302, 2.455, 2.615, 2.772, 0.636, 1.540};
//  0.772 , 0.916 , 1.066, 1.227, 1.390, 1.539, 1.684, 1.844, 1.980, 2.135, 2.310, 2.455, 2.607, 2.775, 0.610
const double yPoint[16] = {0.535, 0.700, 0.851, 1.005, 1.161, 1.317, 1.470, 1.623, 1.780, 1.936, 2.095, 2.245, 2.402, 2.556, 1.162, 1.990};
//  0.529, 0.686, 0.848, 1, 1.159, 1.322, 1.468, 1.630, 1.790, 1.939, 2.090, 2.247, 2.395, 2.552, 1.167, 1.930
// cx2 - 1.139 
// cx4 - 1.771
// cx6 - 2.375
// cx7 - 2.685 
const double cxPoint[7] = {0.830, 1.142, 1.410, 1.760, 2.030, 2.381, 2.690}; // 1.400 -2-->3 , 2.020 - 3-->5, 2.695 -4-->7
const double cyPoint[7] = {0.657, 0.922, 1.220, 1.520, 1.841, 2.160, 2.475};
//waypoints from induct one to goals
std::vector<Goal> one_one_waypoint =   { Goal(px, 1),Goal(py, 1), Goal(cx, 1), Goal(cy, 3), Goal(cx, 3), Goal(cy, 2), Goal(x, 5)};
std::vector<Goal> one_two_waypoint =   { Goal(px, 1),Goal(py, 1), Goal(cx, 1), Goal(cy, 3), Goal(cx, 3), Goal(cy, 2), Goal(x, 6)};
std::vector<Goal> one_three_waypoint = { Goal(px, 1),Goal(py, 1), Goal(cx, 1), Goal(cy, 3), Goal(cx, 5), Goal(cy, 2), Goal(x, 10)};
std::vector<Goal> one_four_waypoint =  { Goal(px, 1),Goal(py, 1), Goal(cx, 1), Goal(cy, 3), Goal(cx, 3), Goal(cy, 4), Goal(x, 5)};
std::vector<Goal> one_five_waypoint =  { Goal(px, 1),Goal(py, 1), Goal(cx, 1), Goal(cy, 3), Goal(cx, 3), Goal(cy, 4), Goal(x, 6)};
std::vector<Goal> one_six_waypoint =   { Goal(px, 1),Goal(py, 1), Goal(cx, 1), Goal(cy, 3), Goal(cx, 5), Goal(cy, 4), Goal(x, 10)};
std::vector<Goal> one_seven_waypoint = { Goal(px, 1),Goal(py, 1), Goal(cx, 1), Goal(cy, 3), Goal(cx, 3), Goal(y, 11), Goal(x, 5)};
std::vector<Goal> one_eight_waypoint = { Goal(px, 1),Goal(py, 1), Goal(cx, 1), Goal(cy, 3), Goal(cx, 3), Goal(y, 11), Goal(x, 6)};
std::vector<Goal> one_nine_waypoint =  { Goal(px, 1),Goal(py, 1), Goal(cx, 1), Goal(cy, 3), Goal(cx, 5), Goal(y, 11), Goal(x, 10)};
//waypoints from induct two to goals
std::vector<Goal> two_one_waypoint =   { Goal(px, 1),Goal(py, -1),Goal(cx, 1), Goal(cy, 5), Goal(cx, 3), Goal(cy, 2), Goal(x, 5)};
std::vector<Goal> two_two_waypoint =   { Goal(px, 1),Goal(py, -1),Goal(cx, 1), Goal(cy, 5), Goal(cx, 3), Goal(cy, 2), Goal(x, 6)};
std::vector<Goal> two_three_waypoint = { Goal(px, 1),Goal(py, -1),Goal(cx, 1), Goal(cy, 5), Goal(cx, 5), Goal(cy, 2), Goal(x, 10)};
std::vector<Goal> two_four_waypoint =  { Goal(px, 1),Goal(py, -1),Goal(cx, 1), Goal(cy, 5), Goal(cx, 3), Goal(cy, 4), Goal(x, 5)};
std::vector<Goal> two_five_waypoint =  { Goal(px, 1),Goal(py, -1),Goal(cx, 1), Goal(cy, 5), Goal(cx, 3), Goal(cy, 4), Goal(x, 6)};
std::vector<Goal> two_six_waypoint =   { Goal(px, 1),Goal(py, -1),Goal(cx, 1), Goal(cy, 5), Goal(cx, 5), Goal(cy, 4), Goal(x, 10)};
std::vector<Goal> two_seven_waypoint = { Goal(px, 1),Goal(py, -1),Goal(cx, 1), Goal(cy, 5), Goal(cx, 3), Goal(y, 11), Goal(x, 5)};
std::vector<Goal> two_eight_waypoint = { Goal(px, 1),Goal(py, -1),Goal(cx, 1), Goal(cy, 5), Goal(cx, 3), Goal(y, 11), Goal(x, 6)};
std::vector<Goal> two_nine_waypoint =  { Goal(px, 1),Goal(py, -1),Goal(cx, 1), Goal(cy, 5), Goal(cx, 5), Goal(cy, 6), Goal(x, 10)};
//waypoints from  goals to  induct one
std::vector<Goal> r_one_zero_waypoint =  { Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
std::vector<Goal> r_one_one_waypoint =   { Goal(cy, 1), Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
std::vector<Goal> r_one_two_waypoint =   { Goal(cy, 1), Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
std::vector<Goal> r_one_three_waypoint = { Goal(cy, 1), Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
std::vector<Goal> r_one_four_waypoint =  { Goal(cy, 1), Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
std::vector<Goal> r_one_five_waypoint =  { Goal(cy, 1), Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
std::vector<Goal> r_one_six_waypoint =   { Goal(cy, 1), Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
std::vector<Goal> r_one_seven_waypoint = { Goal(cy, 1), Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
std::vector<Goal> r_one_eight_waypoint = { Goal(cy, 1), Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
std::vector<Goal> r_one_nine_waypoint =  { Goal(cy, 1), Goal(cx, 1), Goal(y, 15, 1), Goal(x, 15)};
//waypoints from goals to induct two,,,
std::vector<Goal> r_two_zero_waypoint =  { Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
std::vector<Goal> r_two_one_waypoint =   { Goal(cy, 7), Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
std::vector<Goal> r_two_two_waypoint =   { Goal(cy, 7), Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
std::vector<Goal> r_two_three_waypoint = { Goal(cy, 7), Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
std::vector<Goal> r_two_four_waypoint =  { Goal(cy, 7), Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
std::vector<Goal> r_two_five_waypoint =  { Goal(cy, 7), Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
std::vector<Goal> r_two_six_waypoint =   { Goal(cy, 7), Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
std::vector<Goal> r_two_seven_waypoint = { Goal(cy, 7), Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
std::vector<Goal> r_two_eight_waypoint = { Goal(cy, 7), Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
std::vector<Goal> r_two_nine_waypoint =  { Goal(cy, 7), Goal(cx, 1), Goal(y, 16, 1), Goal(x, 15)};
// hash table for paths
std::unordered_map<std::string, std::vector<Goal>> umap = {
    {"1_1", one_one_waypoint},
    {"1_2", one_two_waypoint},
    {"1_3", one_three_waypoint},
    {"1_4", one_four_waypoint},
    {"1_5", one_five_waypoint},
    {"1_6", one_six_waypoint},
    {"1_7", one_seven_waypoint},
    {"1_8", one_eight_waypoint},
    {"1_9", one_nine_waypoint},
    {"2_1", two_one_waypoint},
    {"2_2", two_two_waypoint},
    {"2_3", two_three_waypoint},
    {"2_4", two_four_waypoint},
    {"2_5", two_five_waypoint},
    {"2_6", two_six_waypoint},
    {"2_7", two_seven_waypoint},
    {"2_8", two_eight_waypoint},
    {"2_9", two_nine_waypoint},
    {"r_1_0", r_one_zero_waypoint},
    {"r_1_1", r_one_one_waypoint},
    {"r_1_2", r_one_two_waypoint},
    {"r_1_3", r_one_three_waypoint},
    {"r_1_4", r_one_four_waypoint},
    {"r_1_5", r_one_five_waypoint},
    {"r_1_6", r_one_six_waypoint},
    {"r_1_7", r_one_seven_waypoint},
    {"r_1_8", r_one_eight_waypoint},
    {"r_1_9", r_one_nine_waypoint},
    {"r_2_0", r_two_zero_waypoint},
    {"r_2_1", r_two_one_waypoint},
    {"r_2_2", r_two_two_waypoint},
    {"r_2_3", r_two_three_waypoint},
    {"r_2_4", r_two_four_waypoint},
    {"r_2_5", r_two_five_waypoint},
    {"r_2_6", r_two_six_waypoint},
    {"r_2_7", r_two_seven_waypoint},
    {"r_2_8", r_two_eight_waypoint},
    {"r_2_9", r_two_nine_waypoint},
};
