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
geometry_msgs::TransformStamped transformStamped;
std::vector<Goal> wayPoints = {Goal(x, 0.5)};
double linear_p = 0.5;
double linear_tolerance = 0.5;
void dyn_config();
void callback(flipbot2_base::flipbot2Config config, uint32_t level);
int main(int argc, char **argv) {
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::AsyncSpinner spinner(4);
  spinner.start();
  ros::Publisher pub_cmdVel =
      n.advertise<geometry_msgs::Twist>("flipbot1/cmd_vel", 1000);
  dynamic_reconfigure::Server<flipbot2_base::flipbot2Config> server;
  dynamic_reconfigure::Server<flipbot2_base::flipbot2Config>::CallbackType f;
  f = boost::bind(&callback, _1, _2);
  boost::thread thread_b(updateTransform, &transformStamped, 4);
  server.setCallback(f);
  VelocityController controller(&linear_p, &linear_tolerance,
                                &transformStamped);
  ros::Rate loop_rate(2);
  while (ros::ok()) {
    ROS_INFO("%lf", transformStamped.transform.translation.x);
    controller.setGoal(wayPoints[0]);
    while (!controller.inTolerance()) {
      ROS_INFO("Publishing velocity");
      cmd_msg = controller.calculateVelocity();
      pub_cmdVel.publish(cmd_msg);
      loop_rate.sleep();
  }
    loop_rate.sleep();
  }
  ros::waitForShutdown();

  return 0;
}

void callback(flipbot2_base::flipbot2Config config, uint32_t level) {
  /* ROS_INFO("Reconfigure Request:  %f ", config.proportional_control); */
  linear_p = config.proportional_control;
  linear_tolerance = config.Linear_tolerance;
}
