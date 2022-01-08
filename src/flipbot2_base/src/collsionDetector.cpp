#include "flipbot2_msg/BotGoalAction.h"
#include <actionlib/client/simple_action_client.h>
#include "flipbot2_msg/BotGoalActionFeedback.h"
#include <actionlib/client/terminal_state.h>
#include "flipbot2_msg/BotGoalFeedback.h"
#include "flipbot2_msg/BotInterupt.h"
#include "geometry_msgs/TransformStamped.h"
#include "ros/console_backend.h"
#include "ros/ros.h"
#include "ros/service_client.h"
#include "ros/subscriber.h"
#include "std_msgs/String.h"
#include <string>
#include <tf2_ros/transform_listener.h>
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
          "marker_id" + std::to_string(id2), "marker_id" + std::to_string(id),
          ros::Time(0));
    } catch (tf2::TransformException &ex) {
      ROS_WARN("%s", ex.what());
      ros::Duration(0.5).sleep();
      continue;
    }
  }
}
void callback1(const flipbot2_msg::BotGoalActionFeedbackConstPtr& message){
        bot1Mutex.lock();
        bot1_feedback = message->feedback;
        bot1Mutex.unlock();
}
void callback2(const flipbot2_msg::BotGoalActionFeedbackConstPtr& message){
        bot2Mutex.lock();
        bot2_feedback = message->feedback;
        bot2Mutex.unlock();
}
int main(int argc, char **argv) {
  std::string bot1 = (std::string)argv[1];
  std::string bot2 = (std::string)argv[2];
  bool messagePassed = false;
  ros::init(argc, argv, "collsionDetector"+(std::string)argv[1]+"_"+(std::string)argv[2]);
  ros::NodeHandle n;
  flipbot2_msg::BotInterupt interuptData;
  flipbot2_msg::BotInterupt interuptResumeData;
  interuptData.request.pause =1;
  interuptResumeData.request.pause = 0;
  ros::AsyncSpinner spinner(4);
  spinner.start();
  ros::ServiceClient client =
      n.serviceClient<flipbot2_msg::BotInterupt>("collision_detector");
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac("flipbot"+bot1+"/bot1", true);
  actionlib::SimpleActionClient<flipbot2_msg::BotGoalAction> ac1("flipbot"+bot2+"/bot1", true);
  boost::thread thread_a(updateTransform, &transformMsg, 1, 2);
  ros::Rate loop_rate(10);
  ROS_INFO("Waiting for bot %s action server to start.",argv[1]);
  ac.waitForServer(); // will wait for infinite time
  ROS_INFO("Waiting for bot %s action server to start.",argv[2]);
  ac.waitForServer(); // will wait for infinite time
  ROS_INFO("Starting collsion detectsion");
  ros::Subscriber bot1Feedback = n.subscribe("flipbot"+bot1+"/bot1",1000,callback1);
  while (ros::ok()) {
    if(transformMsg.transform.translation.x < 0.2) {
        client.call(interuptData);
        while(transformMsg.transform.translation.x < 0.2){
                //do nothing until the distance is greater than 0.2
        }
        client.call(interuptResumeData);
        messagePassed = true;
    }
    loop_rate.sleep();
  }
  return 0;
}
