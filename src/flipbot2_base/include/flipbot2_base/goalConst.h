#include "unordered_map"
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
  int point;
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
const double inductX = 0.2;
//                        { 1    , 2    , 3    , 4    , 5    , 6    , 7    , 8    , 9    , 10   , 11   , 12   , 13   , 14, 15}
const double xPoint[15] = { 0.45 , 0.64 , 0.83 , 1.02 , 1.22 , 1.42 , 1.61 , 1.78 , 1.98 , 2.18 , 2.36 , 2.55 , 2.74 , 2.94, 0.25};
const double yPoint[14] = { 0.20 , 0.38 , 0.58 , 0.77 , 0.96 , 1.15 , 1.34 , 1.55 , 1.74 , 1.94 , 2.10 , 2.3  , 2.50 , 2.68};
//waypoints from induct one to goals
std::vector<Goal> one_one_waypoint =   { Goal(x, 3),Goal(y,5)};
std::vector<Goal> one_two_waypoint =   { Goal(x, 7),Goal(y,5)};
std::vector<Goal> one_three_waypoint = { Goal(x, 11),Goal(y,5)};
std::vector<Goal> one_four_waypoint =  {  Goal(x, 3)};
std::vector<Goal> one_five_waypoint =  {  Goal(x, 7)};
std::vector<Goal> one_six_waypoint =   {  Goal(x, 11)};
std::vector<Goal> one_seven_waypoint = {  Goal(x, 2),Goal(y,10),Goal(x,3)};
std::vector<Goal> one_eight_waypoint = {  Goal(x, 2),Goal(y,10),Goal(x,7)};
std::vector<Goal> one_nine_waypoint =  {  Goal(x, 2),Goal(y,10),Goal(x, 11)};
//waypoints from induct two to goals
std::vector<Goal> two_one_waypoint =   { Goal(x, 2),Goal(y,5),Goal(x,3)};
std::vector<Goal> two_two_waypoint =   { Goal(x, 6),Goal(y,5),Goal(x,7)};
std::vector<Goal> two_three_waypoint = { Goal(x,10),Goal(y,5),Goal(x, 11)};
std::vector<Goal> two_four_waypoint =  { Goal(x, 3),Goal(y,9)};
std::vector<Goal> two_five_waypoint =  { Goal(x, 7),Goal(y,9)};
std::vector<Goal> two_six_waypoint =   { Goal(x, 11),Goal(y,9)};
std::vector<Goal> two_seven_waypoint = { Goal(x, 3)};
std::vector<Goal> two_eight_waypoint = { Goal(x, 7)};
std::vector<Goal> two_nine_waypoint =  { Goal(x, 11)};
//waypoints from  goals to  induct one
std::vector<Goal> r_one_one_waypoint =   { Goal(x, 1),Goal(y,5),Goal(x, 15)};
std::vector<Goal> r_one_two_waypoint =   { Goal(x, 1),Goal(y,5),Goal(x, 15)};
std::vector<Goal> r_one_three_waypoint = { Goal(x, 1),Goal(y,5),Goal(x, 15)};
std::vector<Goal> r_one_four_waypoint =  { Goal(y, 5),Goal(x, 1),Goal(y, 5),Goal(x, 15)};
std::vector<Goal> r_one_five_waypoint =  { Goal(y, 5),Goal(x, 1),Goal(y, 5),Goal(x, 15)};
std::vector<Goal> r_one_six_waypoint =   { Goal(y, 5),Goal(x, 1),Goal(y, 5),Goal(x, 15)};
std::vector<Goal> r_one_seven_waypoint = { Goal(x, 2),Goal(y,5),Goal(x,15)};
std::vector<Goal> r_one_eight_waypoint = { Goal(x, 6),Goal(y,5),Goal(x,15)};
std::vector<Goal> r_one_nine_waypoint =  { Goal(x, 10),Goal(y,5),Goal(x,15)};
//waypoints from goals to induct two
std::vector<Goal> r_two_one_waypoint =   { Goal(x, 2),Goal(y,10),Goal(x,15)};
std::vector<Goal> r_two_two_waypoint =   { Goal(x, 6),Goal(y,10),Goal(x,15)};
std::vector<Goal> r_two_three_waypoint = { Goal(x, 10),Goal(y,10),Goal(x,15)};
std::vector<Goal> r_two_four_waypoint =  { Goal(y, 10),Goal(x,1),Goal(x, 15)};
std::vector<Goal> r_two_five_waypoint =  { Goal(y, 10),Goal(x,1),Goal(x, 15)};
std::vector<Goal> r_two_six_waypoint =   { Goal(y, 10),Goal(x,1),Goal(x, 15)};
std::vector<Goal> r_two_seven_waypoint = { Goal(x, 1),Goal(x, 15)};
std::vector<Goal> r_two_eight_waypoint = { Goal(x, 1),Goal(x, 15)};
std::vector<Goal> r_two_nine_waypoint =  { Goal(x, 1),Goal(x, 15)};
std::unordered_map<std::string, std::vector<Goal>> umap = {
        {"1_1",one_one_waypoint},
        {"1_2",one_two_waypoint},
        {"1_3",one_three_waypoint},
        {"1_4",one_four_waypoint},
        {"1_5",one_five_waypoint},
        {"1_6",one_six_waypoint},
        {"1_7",one_seven_waypoint},
        {"1_8",one_eight_waypoint},
        {"1_9",one_nine_waypoint},
        {"2_1",two_one_waypoint},
        {"2_2",two_two_waypoint},
        {"2_3",two_three_waypoint},
        {"2_4",two_four_waypoint},
        {"2_5",two_five_waypoint},
        {"2_6",two_six_waypoint},
        {"2_7",two_seven_waypoint},
        {"2_8",two_eight_waypoint},
        {"2_9",two_nine_waypoint},
        {"r_1_1",r_one_one_waypoint},
        {"r_1_2",r_one_two_waypoint},
        {"r_1_3",r_one_three_waypoint},
        {"r_1_4",r_one_four_waypoint},
        {"r_1_5",r_one_five_waypoint},
        {"r_1_6",r_one_six_waypoint},
        {"r_1_7",r_one_seven_waypoint},
        {"r_1_8",r_one_eight_waypoint},
        {"r_1_9",r_one_nine_waypoint},
        {"r_2_1",r_two_one_waypoint},
        {"r_2_2",r_two_two_waypoint},
        {"r_2_3",r_two_three_waypoint},
        {"r_2_4",r_two_four_waypoint},
        {"r_2_5",r_two_five_waypoint},
        {"r_2_6",r_two_six_waypoint},
        {"r_2_7",r_two_seven_waypoint},
        {"r_2_8",r_two_eight_waypoint},
        {"r_2_9",r_two_nine_waypoint},
};

