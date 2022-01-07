#include "flipbot2_msg/BotGoalAction.h"
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "flipbot2_msg/BotInterupt.h"
#include "geometry_msgs/TransformStamped.h"
#include "ros/console_backend.h"
#include "ros/ros.h"
#include "ros/service_client.h"
#include "std_msgs/String.h"
#include <string>
#include <tf2_ros/transform_listener.h>
geometry_msgs::TransformStamped transformMsg;
/**
 * @brief updates the transform of the robot.Intentionally created to run as
 * thread
 *
 * @param tfBuffer pointer to tf buffer
 * @param _transformstamped pointer to the transform message
 */
void updateTransform(geometry_msgs::TransformStamped *_transformstamped, int id,
                     int id2) {
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);
  while (ros::ok()) {
    try {
      *_transformstamped = tfBuffer.lookupTransform(
          "marker_id" + std::to_string(id2), "marker_id" + std::to_string(id),
          ros::Time(0));
    } catch (tf2::TransformException &ex) {
      ROS_WARN("%s", ex.what());
      ros::Duration(0.5).sleep();
      continue;
    }
  }
}
int main(int argc, char **argv) {
  bool messagePassed = false;
  ros::init(argc, argv, "collsionDetector");
  ros::NodeHandle n;
  flipbot2_msg::BotInterupt interuptData;
  ros::AsyncSpinner spinner(4);
  spinner.start();
  ros::ServiceClient client =
      n.serviceClient<flipbot2_msg::BotInterupt>("collision_detector");
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac("flipbot"+(std::string)argv[1]+"bot1", true);
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac1("flipbot"+(std::string)argv[2]+"bot1", true);
  boost::thread thread_a(updateTransform, &transformMsg, 1, 2);
  ros::Rate loop_rate(10);
  ROS_INFO("Waiting for bot %s action server to start.",argv[1]);
  ac.waitForServer(); // will wait for infinite time
  ROS_INFO("Waiting for bot %s action server to start.",argv[2]);
  ac.waitForServer(); // will wait for infinite time
  while (ros::ok()) {
    while (transformMsg.transform.translation.x < 0.2) {
      if (!messagePassed) {
        interuptData.request.pause = 1;
        client.call(interuptData);
        messagePassed = true;
      }
    }
    loop_rate.sleep();
  }
  return 0;
}
