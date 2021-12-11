 #include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "math.h"
#include <cmath>
geometry_msgs::Twist cmd_msg;
/**
* @brief: calculate the euclidean distance 
*
* @param: int startX
*       : int endX
*
* @return: double
*/
double euclidianDistance(double start,double end)
{
        double distance = 0.0;
        distance = std::sqrt(pow((start-end),2));
        return distance;
}
int main(int argc, char **argv) {
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher pub_cmdVel= n.advertise<geometry_msgs::Twist>("flipbot1/cmd_vel", 1000);
  ros::Rate loop_rate(10);
  cmd_msg.linear.x=0.6;
  while (ros::ok()) {
        pub_cmdVel.publish(cmd_msg);
        ros::spinOnce();
        loop_rate.sleep();
  }
  return 0;
}
