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
#include "flipbot2_base/utils.h"
geometry_msgs::TransformStamped transformMsg;
flipbot2_msg::BotGoalFeedback bot1_feedback;
flipbot2_msg::BotGoalFeedback bot2_feedback;
boost::mutex bot1Mutex;
boost::mutex bot2Mutex;
void callback1(const flipbot2_msg::BotGoalActionFeedbackConstPtr &message)
{
  bot1Mutex.lock();
  bot1_feedback = message->feedback;
  bot1Mutex.unlock();
}
void callback2(const flipbot2_msg::BotGoalActionFeedbackConstPtr &message)
{
  bot2Mutex.lock();
  bot2_feedback = message->feedback;
  bot2Mutex.unlock();
}
int main(int argc, char **argv)
{
  std::string bot1 = (std::string)argv[1];
  std::string bot2 = (std::string)argv[2];
  bool messagePassed = false;
  ros::init(argc, argv,
            "collsionDetector" + (std::string)argv[1] + "_" +
                (std::string)argv[2]);
  ros::NodeHandle n;
  flipbot2_msg::BotInterupt interuptData;
  flipbot2_msg::BotInterupt interuptResumeData;
  interuptData.request.pause = 1;
  interuptResumeData.request.pause = 0;
  ros::AsyncSpinner spinner(1);
  spinner.start();
  ros::ServiceClient client1 =
      n.serviceClient<flipbot2_msg::BotInterupt>("flipbot" + bot1 + "/botStop");
  ros::ServiceClient client2 =
      n.serviceClient<flipbot2_msg::BotInterupt>("flipbot" + bot2 + "/botStop");
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac(
      "flipbot" + bot1 + "/bot1", true);
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac1(
      "flipbot" + bot2 + "/bot1", true);
  ros::Rate loop_rate(5);
  ROS_INFO("Waiting for bot %s action server to start.", argv[1]);
  ac.waitForServer(); // will wait for infinite time
  ROS_INFO("Waiting for bot %s action server to start.", argv[2]);
  ac.waitForServer(); // will wait for infinite time
  ROS_INFO("Starting collsion detectsion for %s , %s", bot1.c_str(), bot2.c_str());
  boost::thread thread_a(updateTransform, &transformMsg, stoi(bot1),
                         stoi(bot2));
  ros::Subscriber bot1Feedback =
      n.subscribe("flipbot" + bot1 + "/bot1/feedback", 1000, callback1);
  ros::Subscriber bot2Feedback =
      n.subscribe("flipbot" + bot2 + "/bot1/feedback", 1000, callback2);
  ros::Duration(1).sleep();
  while (ros::ok())
  {
    
    if (bot1_feedback.axis == "x" && bot2_feedback.axis == "x")
    {
      if (abs(transformMsg.transform.translation.y) < 0.2)
      {
        // check if two bots are perpendicular in x axis (less than size
        // of box[15cm] with offset)
        bool inFront = sgn(transformMsg.transform.translation.x);
        if (abs(transformMsg.transform.translation.x) < 0.3)
        {
          ROS_INFO_NAMED(bot1, "collsion detected in x axis");
          if (inFront)
          {
            ROS_INFO("front collsion detected \n stoping %s", bot1.c_str());
            client1.call(interuptData);
          }
          else
          {
            ROS_INFO("back collsion detected \n stoping %s", bot2.c_str());
            client2.call(interuptData);
          }

          while (true)
          {
            // do nothing until the distance is greater in any axis
            ROS_INFO("y - %lf", transformMsg.transform.translation.y);
            if (fabs(transformMsg.transform.translation.y) > 0.25 ||
                fabs(transformMsg.transform.translation.x) > 0.35)
              break;
            ros::Rate(0.5).sleep();
          }
          if (inFront)
          {
            ROS_INFO("Resuming %s", bot1.c_str());
            client1.call(interuptResumeData);
          }
          else
          {
            ROS_INFO("Resuming %s", bot2.c_str());
            client2.call(interuptResumeData);
          }
        }
      }
    }

    // Y - axis collision detector

    else if (bot1_feedback.axis == "y" && bot2_feedback.axis == "y")
    {

      bool bot1_Direction = sgn(bot1_feedback.yVel);
      bool bot2_Direction = sgn(bot2_feedback.yVel);

      bool Direction = (bot1_Direction && bot2_Direction); // Return 1 if moving away from origin, 0 if moving towards origin
      if(bot1_feedback.point == bot2_feedback.point){

      }
      if (abs(transformMsg.transform.translation.x) < 0.15)
      {
        if (abs(transformMsg.transform.translation.y) < 0.20)
        {
          bool inFront = sgn(transformMsg.transform.translation.y);
          ROS_INFO_NAMED(bot1, "Collission detected in y axis");

          if (Direction) // Bots moving away from Origin
          {
            ROS_INFO("Both in negative");
            if (inFront)
            {
              ROS_INFO("Front collision detected \n stopping %s", bot1.c_str());
              client1.call(interuptData);
            }
            else
            {
              ROS_INFO("Back collision detected \n stopping %s", bot2.c_str());
              client2.call(interuptData);
            }
            while (true)
            {
              // Do nothing until distance is greater in any axis
              if (fabs(transformMsg.transform.translation.x) > 0.20 || fabs(transformMsg.transform.translation.y > 0.15))
              {
                break;
              }
              ros::Rate(0.5).sleep();
            }
            if (inFront)
            {
              ROS_INFO("Resuming %s", bot1.c_str());
              client1.call(interuptResumeData);
            }
            else
            {
              ROS_INFO("Resuming %s", bot2.c_str());
              client2.call(interuptResumeData);
            }
          }
          else
          {
            ROS_INFO("Both in positive");
            if (inFront)
            {
              ROS_INFO("Front collision detected \n stopping %s", bot2.c_str());
              client2.call(interuptData);
            }
            else
            {
              ROS_INFO("Back collision detected \n stopping %s", bot1.c_str());
              client1.call(interuptData);
            }
            while (true)
            {
              // Do nothing until distance is greater in any axis
              if (fabs(transformMsg.transform.translation.x) > 0.20 || fabs(transformMsg.transform.translation.y > 0.20))
              {
                break;
                ros::Rate(0.5).sleep();
              }
            }
            if (inFront)
            {
              ROS_INFO("Resuming %s", bot2.c_str());
              client2.call(interuptResumeData);
            }
            else
            {
              ROS_INFO("Resuming %s", bot1.c_str());
              client1.call(interuptResumeData);
            }
          }
        }
      }
      loop_rate.sleep();
    }
  }
  return 0;
}
