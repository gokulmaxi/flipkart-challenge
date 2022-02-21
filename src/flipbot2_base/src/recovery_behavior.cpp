
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
#include <geometry_msgs/Twist.h>
#include <unistd.h>
#include "goalConst.h"

geometry_msgs::TransformStamped transformMsg;
geometry_msgs::TransformStamped prevTransformMsg;
geometry_msgs::TransformStamped currentTransformMsg;
flipbot2_msg::BotGoalFeedback bot1_feedback;
boost::mutex bot1Mutex;
void callback1(const flipbot2_msg::BotGoalActionFeedbackConstPtr &message)
{
  bot1Mutex.lock();
  bot1_feedback = message->feedback;
  bot1Mutex.unlock();
}
int main(int argc, char **argv)
{
  std::string bot_no = (std::string)argv[1];
  bool messagePassed = false;
  ros::init(argc, argv, "recovery_behavior" + (std::string)argv[1]);
  ros::NodeHandle n;
  flipbot2_msg::BotInterupt interuptData;
  flipbot2_msg::BotInterupt interuptResumeData;
  interuptData.request.pause = 1;
  interuptResumeData.request.pause = 0;
  ros::AsyncSpinner spinner(4);
  spinner.start();
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac(
      "flipbot" + bot_no + "/bot1", true);
  ros::Rate loop_rate(0.5);
  ROS_INFO("Waiting for bot %s action server to start.", argv[1]);
  ac.waitForServer(); // will wait for infinite time
  boost::thread thread_a(updateTransformWorld, &transformMsg, stoi(bot_no));
  ros::Subscriber bot1Feedback =
      n.subscribe("flipbot" + bot_no + "/bot1/feedback", 1000, callback1);
  ros::ServiceClient client =
      n.serviceClient<flipbot2_msg::BotInterupt>("flipbot" + bot_no + "/botStop");
  ros::Publisher pub_cmd =
      n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  geometry_msgs::Twist cmd_vel;
  geometry_msgs::Twist stop;
  ros::Duration(1).sleep();

  while (ros::ok())
  {
    while (bot1_feedback.xVel != 0)
    {
      currentTransformMsg = transformMsg;
      if (abs(prevTransformMsg.transform.translation.x - currentTransformMsg.transform.translation.x) < 0.05)
      {
        ROS_INFO("Starting recovery behaviour for X-Axis");
        // TODO Add mutex if error in reading
        if ((transformMsg.transform.translation.y < cyPoint[3] && currentTransformMsg.transform.translation.y > cyPoint[2]) ||
            (transformMsg.transform.translation.y > cyPoint[4] && currentTransformMsg.transform.translation.y < cyPoint[5]) ||
            (transformMsg.transform.translation.y > cyPoint[6] && currentTransformMsg.transform.translation.y < cyPoint[7]))
        {
          client.call(interuptData);
          cmd_vel.linear.y = 1;
          pub_cmd.publish(cmd_vel);
          ROS_INFO("Moving in +y");
          ros::Duration(1).sleep();
          pub_cmd.publish(stop);
          client.call(interuptResumeData);
        }
        else if (currentTransformMsg.transform.translation.y < cyPoint[2] ||
                 (currentTransformMsg.transform.translation.y > cyPoint[3] && currentTransformMsg.transform.translation.y < cyPoint[4]) ||
                 (currentTransformMsg.transform.translation.y > cyPoint[5] && currentTransformMsg.transform.translation.y < cyPoint[6]))
        {
          client.call(interuptData);
          cmd_vel.linear.y = -1;
          pub_cmd.publish(cmd_vel);
          ROS_INFO("Moving in -y");
          ros::Duration(1).sleep();
          pub_cmd.publish(stop);
          client.call(interuptResumeData);
        }
      }
      else
      {
        ROS_INFO("Recovered bot - Moved in Y-Axis");
      }
      prevTransformMsg = currentTransformMsg;
      loop_rate.sleep();
    }
    while (bot1_feedback.yVel != 0)
    {
      if (abs(prevTransformMsg.transform.translation.y - currentTransformMsg.transform.translation.y) < 0.05)
      {
        ROS_INFO("Starting recovery behaviour for Y-Axis");
        // TODO Add mutex if error in reading
        if (currentTransformMsg.transform.translation.x < cxPoint[2] ||
            (currentTransformMsg.transform.translation.x > cxPoint[3] && currentTransformMsg.transform.translation.x < cxPoint[4]) ||
            (currentTransformMsg.transform.translation.x > cxPoint[5] && currentTransformMsg.transform.translation.x < cxPoint[6]))
        {
          client.call(interuptData);
          cmd_vel.linear.x = -1;
          pub_cmd.publish(cmd_vel);
          ROS_INFO("Moving in -x");
          ros::Duration(1).sleep();
          pub_cmd.publish(stop);
          client.call(interuptResumeData);
        }
        else if ((currentTransformMsg.transform.translation.x < cxPoint[3] && currentTransformMsg.transform.translation.x > cxPoint[2]) ||
                 (currentTransformMsg.transform.translation.x > cxPoint[4] && currentTransformMsg.transform.translation.x < cxPoint[5]) ||
                 (currentTransformMsg.transform.translation.x > cxPoint[6] && currentTransformMsg.transform.translation.x < cxPoint[7]))
        {
          client.call(interuptData);
          cmd_vel.linear.x = 1;
          pub_cmd.publish(cmd_vel);
          ROS_INFO("Moving in +x");
          ros::Duration(1).sleep();
          pub_cmd.publish(stop);
          client.call(interuptResumeData);
        }
      }
      else
      {
        ROS_INFO("Recovered bot - Moved X-Axis");
      }
    }
    loop_rate.sleep();
  }
  return 0;
}
