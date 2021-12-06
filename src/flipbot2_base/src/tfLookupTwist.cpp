#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <ros/console.h>
#include <tf2_ros/transform_listener.h>
#include <turtlesim/Spawn.h>
std::time_t prevTime =0;
double timeDiffrence = 0.0;
double x_prev=0; 
int main(int argc, char **argv) {
  ros::init(argc, argv, "my_tf2_listener");

  ros::NodeHandle node;

  ros::Publisher bot_vel =
      node.advertise<geometry_msgs::Twist>("flipbot2/tf_vel", 10);

  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);

  ros::Rate rate(1);
  while (node.ok()) {
    geometry_msgs::TransformStamped transformStamped;
    try {
      transformStamped =
          tfBuffer.lookupTransform("usb_cam", "marker_id4", ros::Time(0));
    } catch (tf2::TransformException &ex) {
      ROS_WARN("%s", ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }
    std::time_t now_time = std::time(nullptr);
    timeDiffrence=std::difftime(now_time,prevTime);
    double x_now = transformStamped.transform.translation.x;
    double vel_x = (x_now - x_prev)/timeDiffrence;
    ROS_INFO("Current velocuty = %lf",x_now-x_prev);
    prevTime = now_time;
    x_prev = x_now ;
    
    geometry_msgs::Twist vel_msg;

    rate.sleep();
  }
  return 0;
};
