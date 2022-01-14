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
          "marker_id" + std::to_string(id), "marker_id" + std::to_string(id2),
          ros::Time(0));
    } catch (tf2::TransformException &ex) {
      ROS_WARN("%s", ex.what());
      ros::Duration(0.5).sleep();
      continue;
    }
  }
}
bool sgn(double val) { return (0 > val); }
void callback1(const flipbot2_msg::BotGoalActionFeedbackConstPtr &message) {
  bot1Mutex.lock();
  bot1_feedback = message->feedback;
  bot1Mutex.unlock();
}
void callback2(const flipbot2_msg::BotGoalActionFeedbackConstPtr &message) {
  bot2Mutex.lock();
  bot2_feedback = message->feedback;
  bot2Mutex.unlock();
}
int main(int argc, char **argv) {
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
  ros::AsyncSpinner spinner(4);
  spinner.start();
  ros::ServiceClient client1 =
      n.serviceClient<flipbot2_msg::BotInterupt>("flipbot" + bot1 + "/botStop");
  ros::ServiceClient client2 =
      n.serviceClient<flipbot2_msg::BotInterupt>("flipbot" + bot2 + "/botStop");
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac(
      "flipbot" + bot1 + "/bot1", true);
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac1(
      "flipbot" + bot2 + "/bot1", true);
  boost::thread thread_a(updateTransform, &transformMsg, stoi(bot1),
                         stoi(bot2));
  ros::Rate loop_rate(5);
  ROS_INFO("Waiting for bot %s action server to start.", argv[1]);
  /* ac.waitForServer(); // will wait for infinite time */
  ROS_INFO("Waiting for bot %s action server to start.", argv[2]);
  /* ac.waitForServer(); // will wait for infinite time */
  ROS_INFO("Starting collsion detectsion");
  ros::Subscriber bot1Feedback =
      n.subscribe("flipbot" + bot1 + "/bot1", 1000, callback1);
  ros::Duration(1).sleep();
  while (ros::ok()) {
    if (abs(transformMsg.transform.translation.y) < 0.2) {
      // check if two bots are perpendicular in x axis (less than size
      // of box[15cm] with offset)
      ROS_INFO_NAMED(bot1, "y axis aligned");
      bool inFront = sgn(transformMsg.transform.translation.x);
      if (abs(transformMsg.transform.translation.x) < 0.3) {
        ROS_INFO_NAMED(bot1, "collsion detected in x axis");
        if (inFront) {
          ROS_INFO("front collsion detected \n stoping %s", bot1.c_str());
          client1.call(interuptData);
        } else {
          ROS_INFO("back collsion detected \n stoping %s", bot2.c_str());
          client2.call(interuptData);
        }

        while (true) {
          // do nothing until the distance is greater in any axis
          ROS_INFO("y - %lf", transformMsg.transform.translation.y);
          if (fabs(transformMsg.transform.translation.y) > 0.25 ||
              fabs(transformMsg.transform.translation.x) > 0.35)
            break;
          ros::Rate(0.5).sleep();
        }
        if (inFront) {
          ROS_INFO("Resuming %s", bot1.c_str());
          client1.call(interuptResumeData);
        } else {
          ROS_INFO("Resuming %s", bot2.c_str());
          client2.call(interuptResumeData);
        }
      }
    }
    loop_rate.sleep();
  }
  return 0;
}
