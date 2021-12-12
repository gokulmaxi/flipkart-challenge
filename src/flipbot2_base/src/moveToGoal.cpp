#include "flipbot2_base/flipbot.h"
#include "flipbot2_base/flipbot2Config.h"
#include "geometry_msgs/Twist.h"
#include "ros/init.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <boost/core/ref.hpp>
#include <dynamic_reconfigure/server.h>
#include <tf2_ros/transform_listener.h>
geometry_msgs::Twist cmd_msg;
double linear_p = 0.5;
geometry_msgs::TransformStamped transformStamped;
std::vector<Goal> wayPoints = {Goal(x, 2)};

void callback(flipbot2_base::flipbot2Config config, uint32_t level) {
  ROS_INFO("Reconfigure Request:  %f ", config.proportional_control);
  linear_p = config.proportional_control;
}
int main(int argc, char **argv) {
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  dynamic_reconfigure::Server<flipbot2_base::flipbot2Config> server;
  dynamic_reconfigure::Server<flipbot2_base::flipbot2Config>::CallbackType f;
  boost::thread thread_b(updateTransform, &transformStamped,4);
  f = boost::bind(&callback, _1, _2);
  server.setCallback(f);
  ros::Publisher pub_cmdVel =
      n.advertise<geometry_msgs::Twist>("flipbot1/cmd_vel", 1000);
  ros::Rate loop_rate(10);
  cmd_msg.linear.x = 0.6;
  while (ros::ok()) {
    calculateVelocity(wayPoints[1], linear_p);
    ROS_INFO("%lf", transformStamped.transform.translation.x);
    pub_cmdVel.publish(cmd_msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
