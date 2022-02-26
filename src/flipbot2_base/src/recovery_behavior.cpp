#include "flipbot2_base/goalConst.h"
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
#include <geometry_msgs/Twist.h>
#include <string>
#include <tf2_ros/transform_listener.h>
#include <unistd.h>

geometry_msgs::TransformStamped transformMsg;
geometry_msgs::TransformStamped prevTransformMsg;
geometry_msgs::TransformStamped currentTransformMsg;
flipbot2_msg::BotGoalFeedback bot1_feedback;
geometry_msgs::Twist bot1cmd_msg;
boost::mutex bot1Mutex;
void callback1(const flipbot2_msg::BotGoalActionFeedbackConstPtr &message)
{
  bot1Mutex.lock();
  bot1_feedback = message->feedback;
  bot1Mutex.unlock();
}
void vel_callback(const geometry_msgs::Twist &message)
{
  bot1cmd_msg = message;
}
int main(int argc, char **argv)
{
  std::string bot_no = (std::string)argv[1];
  bool messagePassed = false;
  ros::init(argc, argv, "recovery_behavior" + (std::string)argv[1]);
  ros::NodeHandle n;
  flipbot2_msg::BotInterupt interuptData;
  flipbot2_msg::BotInterupt interuptResumeData;
  interuptData.request.pause = 2;
  interuptResumeData.request.pause = 0;
  ros::AsyncSpinner spinner(1);
  spinner.start();
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac(
      "flipbot" + bot_no + "/bot1", true);
  ros::Rate loop_rate(0.4);
  ROS_INFO("Waiting for bot %s action server to start.", argv[1]);
  ac.waitForServer(); // will wait for infinite time
  boost::thread thread_a(updateTransformWorld, &transformMsg, stoi(bot_no));
  ros::Subscriber bot1Feedback =
      n.subscribe("flipbot" + bot_no + "/bot1/feedback", 1000, callback1);
  ros::Subscriber bot1cmd_feedback =
      n.subscribe("flipbot" + bot_no + "/cmd_vel", 1000, vel_callback);
  ros::ServiceClient client = n.serviceClient<flipbot2_msg::BotInterupt>(
      "flipbot" + bot_no + "/botStop");
  ros::Publisher pub_cmd =
      n.advertise<geometry_msgs::Twist>("flipbot" + bot_no + "/cmd_vel", 1000);
  geometry_msgs::Twist cmd_vel;
  geometry_msgs::Twist stop;
  int prevAxis = 0; // 1 for x and 2 for y
  prevTransformMsg = transformMsg;
  ros::Duration(3).sleep();
  while (ros::ok())
  {
    while (bot1cmd_msg.linear.x != 0)
    {
      currentTransformMsg = transformMsg;
      int bot_state;
      ros::param::get("/flipbot" + bot_no + "/bot_state", bot_state);
      if (bot_state == 1)
      {
        if (prevAxis == 1)
        {
          ROS_INFO("transfrom diff = %f",
                   abs(prevTransformMsg.transform.translation.x -
                       currentTransformMsg.transform.translation.x));
          if (abs(prevTransformMsg.transform.translation.x -
                  currentTransformMsg.transform.translation.x) < 0.03)
          {
            ROS_INFO("Starting recovery behaviour for X-Axis");
            // TODO Add mutex if error in reading
            if (bot1_feedback.point == 15)
            {
              if (currentTransformMsg.transform.translation.y > yPoint[7] &&
                  currentTransformMsg.transform.translation.y < yPoint[11])
              {
                ROS_INFO("Bot in Induct 2 Zone range");
                if (currentTransformMsg.transform.translation.y > yPoint[7] &&
                    currentTransformMsg.transform.translation.y < yPoint[9])
                {
                  client.call(interuptData);
                  ros::Duration(0.2).sleep();
                  cmd_vel.linear.x = 0;
                  cmd_vel.linear.y = 1;
                  pub_cmd.publish(cmd_vel);
                  ROS_INFO("Moving in +y - Induct2");
                  ros::Duration(0.4).sleep();
                  pub_cmd.publish(stop);
                  client.call(interuptResumeData);
                }
                else if (currentTransformMsg.transform.translation.y >
                             yPoint[9] &&
                         currentTransformMsg.transform.translation.y <
                             yPoint[11])
                {
                  client.call(interuptData);
                  ros::Duration(0.2).sleep();
                  cmd_vel.linear.x = 0;
                  cmd_vel.linear.y = -1;
                  pub_cmd.publish(cmd_vel);
                  ROS_INFO("Moving in -y - Induct2");
                  ros::Duration(0.4).sleep();
                  pub_cmd.publish(stop);
                  client.call(interuptResumeData);
                }
              }
              else if (currentTransformMsg.transform.translation.y >
                           yPoint[2] &&
                       currentTransformMsg.transform.translation.y <
                           yPoint[6])
              {
                ROS_INFO("Bot in Induct 1 Zone range");
                if (currentTransformMsg.transform.translation.y > yPoint[2] &&
                    currentTransformMsg.transform.translation.y < yPoint[4])
                {
                  client.call(interuptData);
                  ros::Duration(0.2).sleep();
                  cmd_vel.linear.x = 0;
                  cmd_vel.linear.y = 1;
                  pub_cmd.publish(cmd_vel);
                  ROS_INFO(
                      "Moving in +y - Induct-1 p - %f c - %f a - %f", yPoint[2],
                      currentTransformMsg.transform.translation.y, yPoint[4]);
                  ros::Duration(0.4).sleep();
                  pub_cmd.publish(stop);
                  client.call(interuptResumeData);
                }
                else if (currentTransformMsg.transform.translation.y >
                             yPoint[4] &&
                         currentTransformMsg.transform.translation.y <
                             yPoint[6])
                {
                  client.call(interuptData);
                  ros::Duration(0.2).sleep();
                  cmd_vel.linear.x = 0;
                  cmd_vel.linear.y = -1;
                  pub_cmd.publish(cmd_vel);
                  ROS_INFO(
                      "Moving in -y - Induct-1 p - %f c - %f a - %f", yPoint[3],
                      currentTransformMsg.transform.translation.y, yPoint[6]);
                  ros::Duration(0.4).sleep();
                  pub_cmd.publish(stop);
                  client.call(interuptResumeData);
                }
              }
            }
            else if ((currentTransformMsg.transform.translation.y >
                          cyPoint[1] &&
                      currentTransformMsg.transform.translation.y <
                          cyPoint[2]) ||
                     (currentTransformMsg.transform.translation.y >
                          cyPoint[3] &&
                      currentTransformMsg.transform.translation.y <
                          cyPoint[4]) ||
                     (currentTransformMsg.transform.translation.y >
                          cyPoint[5] &&
                      currentTransformMsg.transform.translation.y <
                          cyPoint[6]))
            {
              client.call(interuptData);
              ros::Duration(0.2).sleep();
              cmd_vel.linear.y = 1;
              cmd_vel.linear.x = 0;
              pub_cmd.publish(cmd_vel);
              ROS_INFO("Moving in +y");
              ros::Duration(1).sleep();
              pub_cmd.publish(stop);
              client.call(interuptResumeData);
            }
            else if (currentTransformMsg.transform.translation.y <
                         cyPoint[1] ||
                     (currentTransformMsg.transform.translation.y >
                          cyPoint[2] &&
                      currentTransformMsg.transform.translation.y <
                          cyPoint[3]) ||
                     (currentTransformMsg.transform.translation.y >
                          cyPoint[4] &&
                      currentTransformMsg.transform.translation.y <
                          cyPoint[5]))
            {
              client.call(interuptData);
              ros::Duration(0.2).sleep();
              cmd_vel.linear.y = -1;
              cmd_vel.linear.x = 0;
              pub_cmd.publish(cmd_vel);
              ROS_INFO("Moving in -y");
              ros::Duration(1).sleep();
              pub_cmd.publish(stop);
              client.call(interuptResumeData);
              ros::Duration(1).sleep();
            }
          }
        }
        else
        {
          ROS_INFO("changed axis to x");
          prevAxis = 1;
        }
      }
      else
      {
        prevAxis = 0;
      }
      loop_rate.sleep();
      prevTransformMsg = currentTransformMsg;
    }
    while (bot1cmd_msg.linear.y != 0)
    {
      currentTransformMsg = transformMsg;
      int bot_state;
      ros::param::get("/flipbot" + bot_no + "/bot_state", bot_state);
      if (bot_state == 1)
      {
        if (prevAxis == 2)
        {
          ROS_INFO("transfrom diff = %f velocity %f",
                   abs(prevTransformMsg.transform.translation.y -
                       currentTransformMsg.transform.translation.y),
                   bot1_feedback.yVel);
          if (abs(prevTransformMsg.transform.translation.y -
                  currentTransformMsg.transform.translation.y) < 0.05)
          {
            ROS_INFO("Starting recovery behaviour for Y-Axis");
            // TODO Add mutex if error in reading
            if (currentTransformMsg.transform.translation.x < cxPoint[1] ||
                (currentTransformMsg.transform.translation.x > cxPoint[2] &&
                 currentTransformMsg.transform.translation.x < cxPoint[3]) ||
                (currentTransformMsg.transform.translation.x > cxPoint[4] &&
                 currentTransformMsg.transform.translation.x < cxPoint[5]))
            {
              client.call(interuptData);
              cmd_vel.linear.x = -1;
              cmd_vel.linear.y = 0;
              ros::Duration(0.2).sleep();
              pub_cmd.publish(cmd_vel);
              ROS_INFO("Moving in -x");
              ros::Duration(1).sleep();
              pub_cmd.publish(stop);
              client.call(interuptResumeData);
            }
            else if ((currentTransformMsg.transform.translation.x >
                          cxPoint[1] &&
                      currentTransformMsg.transform.translation.x <
                          cxPoint[2]) ||
                     (currentTransformMsg.transform.translation.x >
                          cxPoint[3] &&
                      currentTransformMsg.transform.translation.x <
                          cxPoint[4]) ||
                     (currentTransformMsg.transform.translation.x >
                          cxPoint[5] &&
                      currentTransformMsg.transform.translation.x <
                          cxPoint[6]))
            {
              client.call(interuptData);
              ros::Duration(0.2).sleep();
              cmd_vel.linear.x = 1;
              cmd_vel.linear.y = 0;
              pub_cmd.publish(cmd_vel);
              ROS_INFO("Moving in +x");
              ros::Duration(1).sleep();
              pub_cmd.publish(stop);
              client.call(interuptResumeData);
            }
          }
        }
        else
        {
          ROS_INFO("changed axis to y");
          prevAxis = 2;
        }
      }
      else
      {
        prevAxis = 0;
      }
      prevTransformMsg = currentTransformMsg;
      loop_rate.sleep();
    }
  }
}
