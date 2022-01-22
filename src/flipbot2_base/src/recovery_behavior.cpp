
#include "flipbot2_base/utils.h"
#include "flipbot2_msg/BotGoalAction.h"
#include "flipbot2_msg/BotGoalActionFeedback.h"
#include "flipbot2_msg/BotGoalFeedback.h"
#include "flipbot2_msg/BotInterupt.h"
#include "geometry_msgs/TransformStamped.h"
#include "ros/console_backend.h"
#include "ros/ros.h"
#include "ros/service_client.h"
#include "ros/subscriber.h"
#include "std_msgs/String.h"
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <cstdlib>
#include <string>
#include <tf2_ros/transform_listener.h>
#include <unistd.h>
geometry_msgs::TransformStamped transformMsg;
geometry_msgs::TransformStamped prevTransformMsg;
flipbot2_msg::BotGoalFeedback bot1_feedback;
boost::mutex bot1Mutex;
void callback1(const flipbot2_msg::BotGoalActionFeedbackConstPtr &message) {
  bot1Mutex.lock();
  bot1_feedback = message->feedback;
  bot1Mutex.unlock();
}
int main(int argc, char **argv) {
  std::string bot1 = (std::string)argv[1];
  bool messagePassed = false;
  ros::init(argc, argv, "recovery_behavior" + (std::string)argv[1]);
  ros::NodeHandle n;
  ros::AsyncSpinner spinner(4);
  spinner.start();
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac(
      "flipbot" + bot1 + "/bot1", true);
  ros::Rate loop_rate(5);
  ROS_INFO("Waiting for bot %s action server to start.", argv[1]);
  ac.waitForServer(); // will wait for infinite time
  boost::thread thread_a(updateTransformWorld, &transformMsg,stoi(bot1));
  ros::Subscriber bot1Feedback =
      n.subscribe("flipbot" + bot1 + "/bot1/feedback", 1000, callback1);
  ros::Duration(1).sleep();

  while (ros::ok()) {
    while (bot1_feedback.xVel != 0) {
      if (prevTransformMsg == transformMsg) {
        ROS_INFO("Starting recovery behaviour");
        //TODO add a new service to start recovery_behavior
      }
    }
    while (bot1_feedback.yVel != 0) {
      if (prevTransformMsg == transformMsg) {
        ROS_INFO("Starting recovery behaviour");
        //TODO add a new service to start recovery_behavior
      }
    }
    loop_rate.sleep();
  }
  return 0;
}
