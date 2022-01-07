#include "flipbot2_msg/BotGoalAction.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Twist.h"
#include "goalConst.h"
#include "math.h"
#include "ros/ros.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include <actionlib/server/simple_action_server.h>
#include <algorithm>
#include <cmath>
#include <exception>
#include <flipbot2_msg/BotGoalFeedback.h>
#include <flipbot2_msg/BotGoalGoal.h>
#include <flipbot2_msg/BotInterupt.h>
#include <flipbot2_msg/BotInteruptRequest.h>
#include <flipbot2_msg/BotInteruptResponse.h>
#include <flipbot2_base/flipbot2Config.h>
#include <string>
#include <strings.h>
#include <tf/tf.h>
#include <unistd.h>
class VelocityController {
private:
  /* double *linearTolerance; */
  /* double *linearP; */
  Goal goal;
  int angularPulse;
  flipbot2_base::flipbot2Config *config;
  geometry_msgs::TransformStamped *transformPtr;
  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<flipbot2_msg::BotGoalAction> as_;
  std::string action_name_;
  geometry_msgs::Twist cmd_msg;
  ros::Publisher pub_cmdVel =
      nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  ros::ServiceServer service = nh_.advertiseService(
      "botStop", &VelocityController::servCallback, this);
  geometry_msgs::Twist stop;
  geometry_msgs::Twist prevMessage;
  int lastDest = 3;
  flipbot2_msg::BotGoalResult result_;
  flipbot2_msg::BotGoalFeedback feedback_;
  boost::mutex BotInteruptMutex;
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
                     flipbot2_base::flipbot2Config *_config, std::string name)
      : as_(nh_, name, boost::bind(&VelocityController::executeCB, this, _1),
            false),
        action_name_(name) {
    transformPtr = _transformPtr;
    angularPulse = _config->angular_pulse;
    this->config = _config;
    //! @TODO find a way to assign zeros at declaration
    stop.linear.x = 0;
    stop.linear.y = 0;
    stop.angular.z = 0;
    prevMessage = stop;
    as_.start();
  }
  ~VelocityController() {
    as_.shutdown();
    pub_cmdVel.shutdown();
  }
  bool servCallback(flipbot2_msg::BotInteruptRequest &req,
                    flipbot2_msg::BotInteruptResponse &res) {
    if (req.pause == 1) {
      pub_cmdVel.publish(stop);
      BotInteruptMutex.lock();
      pub_cmdVel.publish(stop); // additional stop just for safety
      ROS_INFO("Stoping the robot");
    }
    if (req.pause == 0) {
      BotInteruptMutex.unlock();
    }
    return true;
  }
  void executeCB(const flipbot2_msg::BotGoalGoalConstPtr &goal) {
    ros::Rate loop_rate(20);
    bool success = true;
    int induct;
    std::string goalId;
    ROS_INFO("got  the goal %i", goal->index);
    if (goal->index > 0) {
      induct = findInduct();
      goalId = std::to_string(induct) + "_" + std::to_string(abs(goal->index));
    }
    if (goal->index < 0) {
      induct = findNearInduct();
      goalId = "r_" + std::to_string(induct) + "_" +
               std::to_string(abs(goal->index));
    }
    ROS_INFO("Executin plan from %s", goalId.c_str());
    auto hashFound = umap.find(goalId);
    std::vector<Goal> waypoints = hashFound->second;
    for (Goal goalPoint : waypoints) {
      if (as_.isPreemptRequested() || !ros::ok()) {
        ROS_INFO("%s: Preempted", action_name_.c_str());
        // set the action state to preempted
        as_.setPreempted();
        break;
      }
      this->setGoal(goalPoint);
      feedback_.axis = goalPoint.axis;
      as_.publishFeedback(feedback_);
      ROS_INFO("Move in %c to point %i", axisToString(goalPoint.axis),
               goalPoint.point);
      while (!inTolerance()) {
        cmd_msg = calculateVelocity();
        BotInteruptMutex.lock();
        pub_cmdVel.publish(cmd_msg);
        BotInteruptMutex.unlock();
        if (inTolerance()) {
          pub_cmdVel.publish(stop);
          break;
        }
        loop_rate.sleep();
      }
      lastDest = goal->index;
      pub_cmdVel.publish(stop);
    }
    result_.destIndex = goal->index;
    result_.inductIndex = induct;

    as_.setSucceeded(result_);
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
    if (goal.axis == x )
      _distance =
          xPoint[goal.point - 1] - transformPtr->transform.translation.x;
    if (goal.axis == cx )
      _distance =
          cxPoint[goal.point - 1] - transformPtr->transform.translation.x;
    if (goal.axis == y )
      _distance =
          yPoint[goal.point - 1] - transformPtr->transform.translation.y;
    if (goal.axis == cy )
      _distance =
          cyPoint[goal.point - 1] - transformPtr->transform.translation.y;
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
    if (goal.axis == x || goal.axis == cx) {
      double _linearVel = euclidianDistance() * config->proportional_control;
      _twist.linear.x = _linearVel;
    }
    if (goal.axis == y || goal.axis == cy) {
      double _linearVel = euclidianDistance();
      _twist.linear.y = _linearVel;
    }
    if (abs(quatToyaw()) > config->angular_tolerance) {
      if (angularPulse == config->angular_pulse) {
        /* _twist.linear.x = 0; */
        /* _twist.linear.y = 0; */
        _twist.angular.z =
            quatToyaw() * config->angular_constant; // to make the robot turn
        ROS_WARN("Out of angular tolerance");       // the opposite of yaw error
        angularPulse = 0;
      } else {
        angularPulse++;
      }
    } else {
      _twist.angular.z = 0;
    }
    return _twist;
  }
  
  /**
   * @brief finds the current induct point of the robot
   *
   * @return current in point
   */
  int findInduct() {
    if (abs(transformPtr->transform.translation.y - yPoint[4]) < 0.3) {
      return 1;
    } else if (abs(transformPtr->transform.translation.y - yPoint[9]) < 0.3) {
      return 2;
    }
    return 0;
  }

  /**
   * @brief finds the nearest induct point from current position 
   *
   * @return nearest induct point to the robot
   */
  int findNearInduct() {
    if (transformPtr->transform.translation.y < yPoint[6])
      return 1;
    else {
      return 2;
    }
  }

  /**
   * @brief util function to convert axis enum to string
   *
   * @param _axis enum of the axis
   *
   * @return  string of the given enum
   */
  char axisToString(Axis _axis) {
    if (_axis == x || _axis == cx)
      return 'x';
    else
      return 'y';
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
