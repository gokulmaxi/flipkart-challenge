#include "geometry_msgs/TransformStamped.h"
#include "ros/console_backend.h"
#include "ros/ros.h"
#include "ros/service_client.h"
#include "std_msgs/String.h"
#include <tf2_ros/transform_listener.h>
#include "flipbot2_base/BotInterupt.h"
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
  ros::init(argc, argv, "collsionDetector");
  ros::NodeHandle n;
  ros::AsyncSpinner spinner(4);
  ros::ServiceClient client = n.serviceClient<flipbot2_base::BotInterupt>("collision_detector");
  spinner.start();
  boost::thread thread_a(updateTransform, &transformMsg, 1, 2);
  ros::Rate loop_rate(10);
  while (ros::ok()) {
    if(transformMsg.transform.translation.x < 0.2){}
    loop_rate.sleep();
  }
  return 0;
}
