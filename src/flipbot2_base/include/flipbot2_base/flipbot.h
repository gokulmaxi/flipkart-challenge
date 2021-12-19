#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Twist.h"
#include "math.h"
#include "ros/ros.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include <cmath>
#include <exception>
#include <flipbot2_base/flipbot2Config.h>
#include <tf/tf.h>
enum Axis { x, y };
class Goal {
public:
  Axis axis;
  float point;
  Goal() {
    axis = x;
    point = 0;
  }
  Goal(Axis _axis, float _point) {
    axis = _axis;
    point = _point;
  }
};
class VelocityController {
private:
  /* double *linearTolerance; */
  /* double *linearP; */
  Goal goal;
  flipbot2_base::flipbot2Config *config;
  geometry_msgs::TransformStamped *transformPtr;
  int angularPulse;
  /**
   * @brief converts Quaternion to euler angles
   *
   * @return return value of yaw
   */
  double quatToyaw() {
    tf::Quaternion q(
        transformPtr->transform.rotation.x, transformPtr->transform.rotation.y,
        transformPtr->transform.rotation.z, transformPtr->transform.rotation.w);
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    return yaw;
  }

public:
  VelocityController(geometry_msgs::TransformStamped *_transformPtr,
                     flipbot2_base::flipbot2Config *_config) {
    transformPtr = _transformPtr;
    angularPulse = _config->angular_pulse;
    ROS_WARN("INSIDE CONSTRUCTOR");
    this->config = _config;
    ROS_WARN("SIDE CONSTRUCTOR");
  }
  void setGoal(Goal _goal) { this->goal = _goal; }
  /**
   * @brief: calculate the euclidean distance
   *
   * @param: int startX
   *       : int endX
   *
   * @return: double
   */
  double euclidianDistance() {
    double _distance = 0.0;
    /* distance = std::sqrt(pow((start - end), 2)); */
    if (goal.axis == x)
      _distance = goal.point - transformPtr->transform.translation.x;
    if (goal.axis == y)
      _distance = goal.point - transformPtr->transform.translation.y;
    return _distance;
  }
  /**
   * @brief: checks if robot is in tolerance
   *
   * @param: Goal _goal
   *
   * @return: bool
   */
  bool inTolerance() {
    if (abs(euclidianDistance()) < config->Linear_tolerance) {
      return true;
    } else {
      return false;
    }
  }
  geometry_msgs::Twist calculateVelocity() {
    geometry_msgs::Twist _twist;
    if (goal.axis == x) {
      double _linearVel =
           euclidianDistance() * config->proportional_control;
      _twist.linear.x = _linearVel;
    }
    if (goal.axis == y) {
      double _linearVel = euclidianDistance();
      _twist.linear.y = _linearVel;
    }
    if (abs(quatToyaw()) > config->angular_tolerance) {
      if (angularPulse == config->angular_pulse) {
        _twist.linear.x = 0;
        _twist.angular.z = 
            quatToyaw() * config->angular_constant; // to make the robot turn
                                                    // the opposite of yaw error
        angularPulse = 0;
      } else {
        angularPulse++;
      }
    } else {
      _twist.angular.z = 0;
    }
    return _twist;
  }
};
/**
 * @brief updates the transform of the robot.Intentionally created to run as
 * thread
 *
 * @param tfBuffer pointer to tf buffer
 * @param _transformstamped pointer to the transform message
 */
void updateTransform(geometry_msgs::TransformStamped *_transformstamped,
                     int id) {
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);
  while (ros::ok()) {
    try {
      *_transformstamped = tfBuffer.lookupTransform(
          "world", "marker_id" + std::to_string(id), ros::Time(0));
    } catch (tf2::TransformException &ex) {
      ROS_WARN("%s", ex.what());
      ros::Duration(0.5).sleep();
      continue;
    }
  }
}
