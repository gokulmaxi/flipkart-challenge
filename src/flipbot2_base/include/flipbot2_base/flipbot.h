#include "geometry_msgs/TransformStamped.h"
#include "math.h" 
#include <cmath>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
enum Axis { x, y };
class Goal {
public:
  Axis axis;
  float point;
  Goal(Axis _axis, float _point) {
    axis = _axis;
    point = _point;
  }
};
/**
 * @brief: calculate the euclidean distance
 *
 * @param: int startX
 *       : int endX
 *
 * @return: double
 */
double euclidianDistance(double start, double end) {
  double distance = 0.0;
  distance = std::sqrt(pow((start - end), 2));
  return distance;
}

geometry_msgs::Twist calculateVelocity(Goal _goal,double pConstant){
        geometry_msgs::Twist _twist;
        if(_goal.axis == x ){
                ROS_INFO("Moving in X direction");
                double _linearVel = euclidianDistance(0,_goal.point);
                _twist.linear.x = 0;
        }
        if(_goal.axis == y ){
                ROS_INFO("Moving in Y direction");
                double _linearVel = euclidianDistance(0,_goal.point);
                _twist.linear.y = _linearVel * pConstant;
        }

        return _twist;
}


/**
 * @brief updates the transform of the robot.Intentionally created to run as
 * thread
 *
 * @param tfBuffer pointer to tf buffer
 * @param _transformstamped pointer to the transform message
 */
void updateTransform(geometry_msgs::TransformStamped *_transformstamped,int id) {
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);
while(ros::ok()){
  try {
    *_transformstamped =
        tfBuffer.lookupTransform("usb_cam", "marker_id"+std::to_string(id), ros::Time(0));
  } catch (tf2::TransformException &ex) {
    ROS_WARN("%s", ex.what());
    ros::Duration(1.0).sleep();
      continue;
  }
}
}
