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
#include <string>
#include <tf2_ros/transform_listener.h>
#include <unistd.h>
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
  ROS_INFO("Starting collsion detectsion for %s , %s", bot1.c_str(),
           bot2.c_str());
  boost::thread thread_a(updateTransform, &transformMsg, stoi(bot1),
                         stoi(bot2));
  ros::Subscriber bot1Feedback =
      n.subscribe("flipbot" + bot1 + "/bot1/feedback", 1000, callback1);
  ros::Subscriber bot2Feedback =
      n.subscribe("flipbot" + bot2 + "/bot1/feedback", 1000, callback2);
  ros::Duration(1).sleep();
  while (ros::ok())
  {
    int value1;
    int value2;
    ros::param::get("/induct1_wait", value1);
    ros::param::get("/induct2_wait", value2);
    // +Y  -Y Collision
    if ((bot1_feedback.goalPoint == 4 && bot2_feedback.goalPoint == 4) ||
        (bot1_feedback.goalPoint == 5 && bot2_feedback.goalPoint == 5) ||
        (bot1_feedback.goalPoint == 4 && bot2_feedback.goalPoint == 5) ||
        (bot1_feedback.goalPoint == 5 && bot2_feedback.goalPoint == 4) ||
        (bot1_feedback.goalPoint == 2 && bot2_feedback.goalPoint == 8 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 8 && bot2_feedback.goalPoint == 2 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 1 && bot2_feedback.goalPoint == 7 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 7 && bot2_feedback.goalPoint == 1 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 4 && bot2_feedback.goalPoint == 8 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 8 && bot2_feedback.goalPoint == 4 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 4 && bot2_feedback.goalPoint == 7 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 7 && bot2_feedback.goalPoint == 4 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 5 && bot2_feedback.goalPoint == 7 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 7 && bot2_feedback.goalPoint == 5 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 5 && bot2_feedback.goalPoint == 8 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 8 && bot2_feedback.goalPoint == 5 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 4 && bot2_feedback.goalPoint == 2 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 2 && bot2_feedback.goalPoint == 4 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 4 && bot2_feedback.goalPoint == 1 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 1 && bot2_feedback.goalPoint == 4 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 5 && bot2_feedback.goalPoint == 2 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 2 && bot2_feedback.goalPoint == 5 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 5 && bot2_feedback.goalPoint == 1 && bot2_feedback.inductPoint == 2 && bot1_feedback.inductPoint == 1) ||
        (bot1_feedback.goalPoint == 1 && bot2_feedback.goalPoint == 5 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint == 1))
    {
      if (bot1_feedback.Xpoint >= xPoint[1])
      {
        ROS_INFO("Moving the bot to waiting zone");
        while (bot2_feedback.Xpoint < xPoint[1])
        {
        }
        client2.call(interuptData);
        while ((bot1_feedback.Ypoint > yPoint[4]) &&
               (bot1_feedback.Ypoint < yPoint[10]))
        {
        }
        ROS_INFO("Resumed client 2");
        client2.call(interuptResumeData);
      }

      else if (bot2_feedback.Xpoint >= xPoint[1])
      {
        ROS_INFO("Moving the bot to waiting zone");
        while (bot1_feedback.Xpoint < xPoint[1])
        {
        }
        client1.call(interuptData);
        while ((bot2_feedback.Ypoint > yPoint[4]) &&
               (bot2_feedback.Ypoint < yPoint[10]))
        {
        }
        ROS_INFO("Resumed client 1");
        client1.call(interuptResumeData);
      }

      else if (value2 > 0)
      {
        ROS_INFO("Bot Waiting in induct 2 - Moving bot in Induct 2");
        if (bot1_feedback.inductPoint == 1)
        {
          ROS_INFO("Moving the bot to waiting zone");
          while (bot1_feedback.Xpoint < xPoint[1])
          {
          }
          ROS_INFO("Moved the bot - 1");
          client1.call(interuptData);
          ROS_INFO("Stoping the bot");
          while ((bot2_feedback.Ypoint > yPoint[4]) &&
                 (bot2_feedback.Ypoint < yPoint[10]))
          {
            // ROS_INFO("Within Colliding zone");
          }
          ROS_INFO("Resuming the bot - Collision avoided");
          client1.call(interuptResumeData);
        }
        else if (bot2_feedback.inductPoint == 1)
        {
          ROS_INFO("Moving the bot to waiting zone");
          while (bot2_feedback.Xpoint <= xPoint[1])
          {
          }
          ROS_INFO("Moved the bot - 2");
          client2.call(interuptData);
          ROS_INFO("Stoping the bot");
          while ((bot1_feedback.Ypoint > yPoint[4]) &&
                 (bot1_feedback.Ypoint < yPoint[10]))
          {
            // ROS_INFO("Within Colliding zone");
          }
          ROS_INFO("Resuming the bot - Collision avoided");
          client2.call(interuptResumeData);
        }
      }
      else
      {
        ROS_INFO("Bot Waiting in induct 1 - Moving bot in Induct 1");

        if (bot1_feedback.inductPoint == 2)
        {
          while (bot1_feedback.Xpoint <= xPoint[1])
          {
            // ROS_INFO("Moving the bot to  waiting zone");
          }
          ROS_INFO("Moved the bot - 3");
          client1.call(interuptData);
          while ((bot2_feedback.Ypoint > yPoint[4]) &&
                 (bot2_feedback.Ypoint < yPoint[10]))
          {
            // ROS_INFO("Within Colliding zone");
          }
          ROS_INFO("Resuming the bot - Collision avoided %f",
                   transformMsg.transform.translation.y);
          client1.call(interuptResumeData);
        }
        else if (bot2_feedback.inductPoint == 2)
        {
          while (bot2_feedback.Xpoint <= xPoint[1])
          {
            // ROS_INFO("Moving the bot to  waiting zone");
          }
          ROS_INFO("Moved the bot - 4");
          client2.call(interuptData);
          while ((bot1_feedback.Ypoint > yPoint[4]) &&
                 (bot1_feedback.Ypoint < yPoint[10]))
          {
            // ROS_INFO("Within Colliding zone");
          }
          ROS_INFO("Resuming the bot - Collision avoided %f",
                   transformMsg.transform.translation.y);
          client2.call(interuptResumeData);
        }
      }
    }
    else if ((bot1_feedback.goalPoint == 6 && bot2_feedback.goalPoint == 6) ||
             (bot1_feedback.goalPoint == 3 && bot2_feedback.goalPoint == 9 && bot1_feedback.inductPoint == 2 && bot2_feedback.inductPoint ==1) ||
             (bot1_feedback.goalPoint == 9 && bot2_feedback.goalPoint == 3 && bot1_feedback.inductPoint == 1 && bot2_feedback.inductPoint ==2))
    {
      if (bot1_feedback.Xpoint >= xPoint[6])
      {
        ROS_INFO("Moving the bot to waiting zone for Goal 6");
        while (bot2_feedback.Xpoint < xPoint[6])
        {
        }
        client2.call(interuptData);
        while ((bot1_feedback.Ypoint > yPoint[4]) &&
               (bot1_feedback.Ypoint < yPoint[10]))
        {
        }
        ROS_INFO("Resumed client 2 for goal 6");
        client2.call(interuptResumeData);
      }
      else if (bot2_feedback.Xpoint >= xPoint[6])
      {
        ROS_INFO("Moving the bot to waiting zone for Goal 6");
        while (bot1_feedback.Xpoint < xPoint[6])
        {
        }
        client1.call(interuptData);
        while ((bot2_feedback.Ypoint > yPoint[4]) &&
               (bot2_feedback.Ypoint < yPoint[10]))
        {
        }
        ROS_INFO("Resumed client 1 for goal 6");
        client1.call(interuptResumeData);
      }
      else if (value2 > 0)
      {
        ROS_INFO("Bot Waiting in induct 2 - Moving bot in Induct 2");
        if (bot1_feedback.inductPoint == 1)
        {
          ROS_INFO("Moving the bot to waiting zone");
          while (bot1_feedback.Xpoint < xPoint[6])
          {
          }
          ROS_INFO("Moved the bot - 1(Goal 6)");
          client1.call(interuptData);
          ROS_INFO("Stoping the bot");
          while ((bot2_feedback.Ypoint > yPoint[4]) &&
                 (bot2_feedback.Ypoint < yPoint[10]))
          {
            // ROS_INFO("Within Colliding zone");
          }
          ROS_INFO("Resuming the bot - Collision avoided");
          client1.call(interuptResumeData);
        }
        else if (bot2_feedback.inductPoint == 1)
        {
          ROS_INFO("Moving the bot to waiting zone");
          while (bot2_feedback.Xpoint <= xPoint[6])
          {
          }
          ROS_INFO("Moved the bot - 2(goal 6)");
          client2.call(interuptData);
          ROS_INFO("Stoping the bot");
          while ((bot1_feedback.Ypoint > yPoint[4]) &&
                 (bot1_feedback.Ypoint < yPoint[10]))
          {
            // ROS_INFO("Within Colliding zone");
          }
          ROS_INFO("Resuming the bot - Collision avoided");
          client2.call(interuptResumeData);
        }
      }
      else
      {
        ROS_INFO("Bot Waiting in induct 1 - Moving bot in Induct 1 (Goal 6)");

        if (bot1_feedback.inductPoint == 2)
        {
          while (bot1_feedback.Xpoint <= xPoint[6])
          {
            // ROS_INFO("Moving the bot to  waiting zone");
          }
          ROS_INFO("Moved the bot - 3 (Goal 6)");
          client1.call(interuptData);
          while ((bot2_feedback.Ypoint > yPoint[4]) &&
                 (bot2_feedback.Ypoint < yPoint[10]))
          {
            // ROS_INFO("Within Colliding zone");
          }
          ROS_INFO("Resuming the bot - Collision avoided %f",
                   transformMsg.transform.translation.y);
          client1.call(interuptResumeData);
        }
        else if (bot2_feedback.inductPoint == 2)
        {
          while (bot2_feedback.Xpoint <= xPoint[6])
          {
            // ROS_INFO("Moving the bot to  waiting zone");
          }
          ROS_INFO("Moved the bot - 4(Goal 6)");
          client2.call(interuptData);
          while ((bot1_feedback.Ypoint > yPoint[4]) &&
                 (bot1_feedback.Ypoint < yPoint[10]))
          {
            // ROS_INFO("Within Colliding zone");
          }
          ROS_INFO("Resuming the bot - Collision avoided %f",
                   transformMsg.transform.translation.y);
          client2.call(interuptResumeData);
        }
      }
    }

    // X- Axis collision

    if (bot1_feedback.axis == "x" && bot2_feedback.axis == "x")
    {
      bool bot1_Direction = sgn(bot1_feedback.xVel);
      bool bot2_Direction = sgn(bot2_feedback.xVel);

      bool Direction =
          (bot1_Direction &&
           bot2_Direction); // Retrun 1 if both are moving away from origin
                            // 0 if moving towards origin
      if (abs(transformMsg.transform.translation.y) < 0.20)
      {
        // check if two bots are perpendicular in x axis (less than size
        // of box[15cm] with offset)
        if (abs(transformMsg.transform.translation.x) < 0.3)
        {
          bool inFront = sgn(transformMsg.transform.translation.x);
          ROS_INFO_NAMED(bot1, "collsion detected in x axis");
          if (Direction)
          {
            ROS_INFO("Both in negative");
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
              // ROS_INFO("y - %lf", transformMsg.transform.translation.y);
              if (fabs(transformMsg.transform.translation.y) > 0.25 ||
                  fabs(transformMsg.transform.translation.x) > 0.35)
                break;
            }
            ros::Rate(0.5).sleep();
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
              ROS_INFO("Front collision detected \n stoping %s", bot2.c_str());
              client2.call(interuptData);
            }
            else
            {
              ROS_INFO("Back collision detected \n stopping %s", bot1.c_str());
              client1.call(interuptData);
            }
            while (true)
            {
              // do nothing until the distance is greater in any axis
              // ROS_INFO("y - %lf", transformMsg.transform.translation.y);
              if (fabs(transformMsg.transform.translation.y) > 0.25 ||
                  fabs(transformMsg.transform.translation.x) > 0.35)
                break;
              ros::Rate(0.5).sleep();
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
    }

    // Y - axis collision detector

    if (bot1_feedback.axis == "y" && bot2_feedback.axis == "y")
    {
      ROS_INFO("( Y , Y");

      bool bot1_Direction = sgn(bot1_feedback.yVel);
      bool bot2_Direction = sgn(bot2_feedback.yVel);
      bool Direction = (bot1_Direction &&
                        bot2_Direction); // Return 1 if moving away from origin,
                                         // 0 if moving towards origin
      if (abs(transformMsg.transform.translation.x) < 0.15)
      {
        ROS_INFO("Withing x zone");
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
              if (fabs(transformMsg.transform.translation.x) > 0.25 ||
                  fabs(transformMsg.transform.translation.y > 0.35))
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
              if (fabs(transformMsg.transform.translation.x) > 0.20 ||
                  fabs(transformMsg.transform.translation.y > 0.20))
                break;
              ros::Rate(0.5).sleep();
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
    }
    // x --- y Collision
    /* else if ((bot1_feedback.axis == "x" && bot2_feedback.axis == "y ") ||
       (bot1_feedback.axis == "y" && bot2_feedback.axis == "x")) */
    else if (false)
    {
      // struct intersectPoint bot1StartPoint, bot1EndPoint, bot2StartPoint,
      // bot2EndPoint; if (bot1_lfeedback.axis == "x")
      // {
      //   float dir = (bot1_lfeedback.xVel < 0) ? 1 : -1;
      //   bot1StartPoint.x = bot1_lfeedback.Xpoint + (0.1 * dir); // to
      //   compensate negative errors bot1StartPoint.y = bot1_lfeedback.Ypoint;
      //   bot1EndPoint.x = bot1_lfeedback.Xpoint + 0.3 * ((bot1_lfeedback.xVel
      //   < 0) ? -1 : 1); bot1EndPoint.y = bot1_lfeedback.Ypoint;
      // }
      // else if (bot1_lfeedback.axis == "y")
      // {
      //   bot1StartPoint.x = bot1_lfeedback.Xpoint; // to compensate negative
      //   errors bot1StartPoint.y = bot1_lfeedback.Ypoint + 0.1 *
      //   ((bot1_lfeedback.yVel < 0) ? 1 : -1); bot1EndPoint.x =
      //   bot1_lfeedback.Xpoint; bot1EndPoint.y = bot1_lfeedback.Ypoint + 0.3 *
      //   ((bot1_feedback.yVel < 0) ? -1 : 1);
      // }
      // if (bot2_lfeedback.axis == "x")
      // {
      //   bot2StartPoint.x = bot2_lfeedback.Xpoint + 0.1 *
      //   ((bot2_lfeedback.xVel < 0) ? 1 : -1); // to compensate negative
      //   errors bot2StartPoint.y = bot2_lfeedback.Ypoint; bot2EndPoint.x =
      //   bot2_lfeedback.Xpoint + 0.3 * ((bot2_lfeedback.xVel < 0) ? -1 : 1);
      //   bot2EndPoint.y = bot2_lfeedback.Ypoint;
      // }
      // else if (bot2_lfeedback.axis == "y")
      // {
      //   bot2StartPoint.x = bot2_lfeedback.Xpoint; // to compensate negative
      //   errors bot2StartPoint.y = bot2_lfeedback.Ypoint + 0.1 *
      //   ((bot2_lfeedback.yVel < 0) ? 1 : -1); bot2EndPoint.x =
      //   bot2_lfeedback.Xpoint; bot2EndPoint.y = bot2_lfeedback.Ypoint + 0.3 *
      //   ((bot2_feedback.yVel < 0) ? -1 : 1);
      // }
      // // check for intersection
      // if (doIntersect(bot1StartPoint, bot2StartPoint, bot1EndPoint,
      // bot2EndPoint))
      // {
      //   ROS_INFO("x - y intersection found - stoping bot %s", bot1.c_str());
      //   ROS_INFO("s- p1 %f p2 %f p3 %f p4 %f", bot1StartPoint.x,
      //   bot1StartPoint.y, bot2StartPoint.x, bot2StartPoint.y); ROS_INFO("e-
      //   p1 %f p2 %f p3 %f p4 %f", bot1EndPoint.x, bot1EndPoint.y,
      //   bot2EndPoint.x, bot2EndPoint.y); client1.call(interuptData); while
      //   (abs(transformMsg.transform.translation.x) < 0.4 &&
      //   abs(transformMsg.transform.translation.y < 0.4))
      //   {
      //     /* Nothing to do here ;) */
      //   }
      //   ROS_INFO("x - y intersection cleared - starting bot %s ",
      //   bot1.c_str()); client1.call(interuptResumeData);
      // }
      ROS_INFO("( X , Y");
      if ((bot1_feedback.xVel > 0 && bot2_feedback.yVel < 0))
      {
        double bot1_fut_Xpoint = (bot1_feedback.Xpoint + 0.30);
        double bot2_fut_Ypoint = (bot2_feedback.Ypoint - 0.30);
        if ((abs(bot1_fut_Xpoint - bot2_feedback.Xpoint) < 0.15) &&
            (abs(bot2_fut_Ypoint - bot1_feedback.Ypoint) < 0.15))
        // compare x and y
        {
          client1.call(interuptData);
          ROS_INFO("Stopped bot - Predicted future collision ( +X , -Y");
        }
        while (abs(transformMsg.transform.translation.x) < 0.4 &&
               abs(transformMsg.transform.translation.y < 0.4))
        {
        }
        ROS_INFO("Collision Avoided");
        client1.call(interuptResumeData);
      }
      else if ((bot1_feedback.xVel < 0 && bot2_feedback.yVel > 0))
      {
        double bot1_fut_Xpoint = (bot1_feedback.Xpoint - 0.30);
        double bot2_fut_Ypoint = (bot2_feedback.Ypoint + 0.30);
        if ((abs(bot1_fut_Xpoint - bot2_feedback.Xpoint) < 0.15) &&
            (abs(bot2_fut_Ypoint - bot1_feedback.Ypoint) < 0.15))
        // compare x and y
        {
          client1.call(interuptData);
          ROS_INFO("Stopped bot - Predicted future collision ( -X, +Y");
        }
        while (abs(transformMsg.transform.translation.x) < 0.4 &&
               abs(transformMsg.transform.translation.y < 0.4))
        {
        }
        ROS_INFO("Collision Avoided");
        client1.call(interuptResumeData);
      }
      //
      else if ((bot2_feedback.xVel < 0 && bot2_feedback.yVel < 0))
      {
        double bot1_fut_Xpoint = (bot1_feedback.Xpoint - 0.30);
        double bot2_fut_Ypoint = (bot2_feedback.Ypoint - 0.30);
        if ((abs(bot1_fut_Xpoint - bot2_feedback.Xpoint) < 0.15) &&
            (abs(bot2_fut_Ypoint - bot1_feedback.Ypoint) < 0.15))
        // compare x and y
        {
          client1.call(interuptData);
          ROS_INFO("Stopped bot - Predicted future collision (-X, -Y");
        }
        while (abs(transformMsg.transform.translation.x) < 0.4 &&
               abs(transformMsg.transform.translation.y) < 0.4)
        {
        }
        ROS_INFO("Collision Avoided");
        client1.call(interuptResumeData);
      }
      //
      else if ((bot2_feedback.xVel > 0 && bot2_feedback.yVel > 0))
      {
        double bot1_fut_Xpoint = (bot1_feedback.Xpoint + 0.30);
        double bot2_fut_Ypoint = (bot2_feedback.Ypoint + 0.30);
        if ((abs(bot1_fut_Xpoint - bot2_feedback.Xpoint) < 0.15) &&
            (abs(bot2_fut_Ypoint - bot1_feedback.Ypoint) < 0.15))
        // compare x and y
        {
          client1.call(interuptData);
          ROS_INFO("Stopped bot - Predicted future collision (+X, +Y");
        }
        while (abs(transformMsg.transform.translation.x) < 0.4 &&
               abs(transformMsg.transform.translation.y) < 0.4)
        {
        }
        ROS_INFO("Collision Avoided");
        client1.call(interuptResumeData);
      }
      //
      else if ((bot1_feedback.yVel > 0 && bot1_feedback.xVel < 0))
      {
        double bot1_fut_Ypoint = (bot1_feedback.Ypoint + 0.30);
        double bot2_fut_Xpoint = (bot2_feedback.Xpoint - 0.30);
        if ((abs(bot1_fut_Ypoint - bot2_feedback.Ypoint) < 0.15) &&
            (abs(bot2_fut_Xpoint - bot1_feedback.Xpoint) < 0.15))
        // compare x and y
        {
          client1.call(interuptData);
          ROS_INFO("Stopped bot - Predicted future collision (+Y, -X");
        }
        while (abs(transformMsg.transform.translation.x) < 0.4 &&
               abs(transformMsg.transform.translation.y) < 0.4)
        {
        }
        ROS_INFO("Collision Avoided");
        client1.call(interuptResumeData);
      }
      //
      else if ((bot1_feedback.yVel < 0 && bot1_feedback.xVel > 0))
      {
        double bot1_fut_Ypoint = (bot1_feedback.Ypoint - 0.30);
        double bot2_fut_Xpoint = (bot2_feedback.Xpoint + 0.30);
        if ((abs(bot1_fut_Ypoint - bot2_feedback.Ypoint) < 0.15) &&
            (abs(bot2_fut_Xpoint - bot1_feedback.Xpoint) < 0.15))
        // compare x and y
        {
          client1.call(interuptData);
          ROS_INFO("Stopped bot - Predicted future collision (-Y, +X");
        }
        while (abs(transformMsg.transform.translation.x) < 0.4 &&
               abs(transformMsg.transform.translation.y) < 0.4)
        {
        }
        ROS_INFO("Collision Avoided");
        client1.call(interuptResumeData);
      }
      //
      else if ((bot1_feedback.yVel < 0 && bot1_feedback.xVel < 0))
      {
        double bot1_fut_Ypoint = (bot1_feedback.Ypoint - 0.30);
        double bot2_fut_Xpoint = (bot2_feedback.Xpoint - 0.30);
        if ((abs(bot1_fut_Ypoint - bot2_feedback.Ypoint) < 0.15) &&
            (abs(bot2_fut_Xpoint - bot1_feedback.Xpoint) < 0.15))
        // compare x and y
        {
          client1.call(interuptData);
          ROS_INFO("Stopped bot - Predicted future collision (-Y, -X");
        }
        while (abs(transformMsg.transform.translation.x) < 0.4 &&
               abs(transformMsg.transform.translation.y) < 0.4)
        {
        }
        ROS_INFO("Collision Avoided");
        client1.call(interuptResumeData);
      }
      //
      else if ((bot1_feedback.yVel > 0 && bot1_feedback.xVel > 0))
      {
        double bot1_fut_Ypoint = (bot1_feedback.Ypoint + 0.30);
        double bot2_fut_Xpoint = (bot2_feedback.Xpoint + 0.30);
        if ((abs(bot1_fut_Ypoint - bot2_feedback.Ypoint) < 0.15) &&
            (abs(bot2_fut_Xpoint - bot1_feedback.Xpoint) < 0.15))
        // compare x and y
        {
          client1.call(interuptData);
          ROS_INFO("Stopped bot - Predicted future collision (+Y, +X");
        }
        while (abs(transformMsg.transform.translation.x) < 0.4 &&
               abs(transformMsg.transform.translation.y) < 0.4)
        {
        }
        ROS_INFO("Collision Avoided");
        client1.call(interuptResumeData);
      }
    }
    loop_rate.sleep();
  }
  return 0;
}
