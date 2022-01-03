#include "flipbot2_base/flipbot.h"
#include "flipbot2_base/flipbot2Config.h"
#include "geometry_msgs/Twist.h"
#include "ros/init.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <boost/core/ref.hpp>
#include <dynamic_reconfigure/server.h>
#include <tf2_ros/transform_listener.h>
geometry_msgs::TransformStamped transformStamped;
double *linear_constant = new double;
double *linear_tolerance = new double;
flipbot2_base::flipbot2Config configGlobal;
geometry_msgs::Twist stop;
void dynamicConfigureCb(flipbot2_base::flipbot2Config config, uint32_t level);
int main(int argc, char **argv) {
  // node and other initialisation
  ros::init(argc, argv, "goal_action_server");
  configGlobal.Linear_tolerance = *linear_tolerance;
  configGlobal.proportional_control = *linear_constant;
  ros::AsyncSpinner spinner(4);
  spinner.start();
  dynamic_reconfigure::Server<flipbot2_base::flipbot2Config> server;
  dynamic_reconfigure::Server<flipbot2_base::flipbot2Config>::CallbackType f;
  f = boost::bind(&dynamicConfigureCb, _1, _2);
  boost::thread thread_b(updateTransform, &transformStamped, 1);
  server.setCallback(f);
  VelocityController controller(&transformStamped, &configGlobal, "flipbot1/bot1");
  ros::Rate loop_rate(20);
  ros::waitForShutdown();
  return 0;
}

void dynamicConfigureCb(flipbot2_base::flipbot2Config config, uint32_t level) {
  ROS_INFO("Reconfigure Request ");
  *linear_constant = config.proportional_control;
  *linear_tolerance = config.Linear_tolerance;
  configGlobal = config;
}
