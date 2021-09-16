import rospy
from turtlesim.msg import Pose
from geometry_msgs.msg import Twist
from math import pow, atan2, sqrt
import math
import time
target = 75
class FlipBot:
    def __init__(self,bot_index:int):
        self.bot_index=bot_index
        self.velocity_publisher = rospy.Publisher(
            '/flipbot'+str(self.bot_index)+'/cmd_vel', Twist, queue_size=10)
        self.pose_subscriber = rospy.Subscriber(
            '/flipbot'+str(self.bot_index)+'/pose', Pose, self.update_pose)
        self.pose = Pose()
        self.rate = rospy.Rate(10)
        self.r2 = rospy.Rate(5)
        self.yaw = 0
        self.rotate_tolerance= 10
    def direction(self,boolean:bool):
        if boolean:
            return 1
        else:
            return -1
    def update_pose(self,data:float):
        self.pose = data
        self.pose.x = round(self.pose.x, 4)
        self.pose.y = round(self.pose.y, 4)
        self.pose.theta = round(data.theta,2)
    def euclidean_distance(self, goal_pose):
        return sqrt(pow((goal_pose.y - self.pose.y), 2))
    def linear_vel(self, goal_pose, constant=0.5):
        return constant * self.euclidean_distance(goal_pose)
    def euclidean_distance_x(self, goal_pose):
        return sqrt(pow((goal_pose.x - self.pose.x), 2))
    def linear_vel_x(self, goal_pose, constant=0.5):
        return constant * self.euclidean_distance_x(goal_pose)
    def steering_angle(self, goal_pose):
        return atan2(goal_pose.y - self.pose.y, goal_pose.x - self.pose.x)
    def angular_vel(self, goal_pose, constant=0.3):
        rospy.loginfo("Current angle --> %f required angular velocity --> %f",self.pose.theta,(self.steering_angle(goal_pose) - self.pose.theta)*0.05)
        return constant * (self.steering_angle(goal_pose) - self.pose.theta)
    def move_y(self,x:float,y:float,direction:bool):
        goal_pose = Pose()
        goal_pose.x = x
        goal_pose.y = y
        distance_tolerance = 0.09
        vel_msg = Twist()
        while self.euclidean_distance(goal_pose) >= distance_tolerance:
            vel_msg.linear.x = self.linear_vel(goal_pose)*self.direction(direction)
            rospy.loginfo("linear velocity Move y --> %f goal distance --> %f",vel_msg.linear.x,self.euclidean_distance(goal_pose))
            vel_msg.linear.y = 0
            vel_msg.linear.z = 0
            # Angular velocity in the z-axis.
            vel_msg.angular.x = 0
            vel_msg.angular.y = 0
            vel_msg.angular.z = 0
            # Publishing our vel_msg
            self.velocity_publisher.publish(vel_msg)
            # Publish at the desired rate.
            self.rate.sleep()
            # Stopping our robot after the movement is over.
        vel_msg.linear.x = 0
        vel_msg.angular.z = 0
        self.velocity_publisher.publish(vel_msg)
             # If we press control + C, the node will stop.

    def move_x(self,x:float,y:float,direction:bool):
        """Moves the turtle to the goal."""
        goal_pose = Pose()
        goal_pose.x = x
        goal_pose.y = y
        distance_tolerance = 0.05
        vel_msg = Twist()
        while self.euclidean_distance_x(goal_pose) >= distance_tolerance:
            # Porportional controller.
            # https://en.wikipedia.org/wiki/Proportional_control
            # Linear velocity in the x-axis.
            vel_msg.linear.x = self.linear_vel_x(goal_pose)*self.direction(direction)
            vel_msg.linear.y = 0
            vel_msg.linear.z = 0
            # Angular velocity in the z-axis.
            vel_msg.angular.x = 0
            vel_msg.angular.y = 0
            vel_msg.angular.z = 0
            rospy.loginfo("linear velocity Move x --> %f goal distance --> %f",vel_msg.linear.x,self.euclidean_distance_x(goal_pose))
            # Publishing our vel_msg
            self.velocity_publisher.publish(vel_msg)
            # Publish at the desired rate.
            self.rate.sleep()
        # Stopping our robot after the movement is over.
        vel_msg.linear.x = 0
        vel_msg.angular.z = 0
        self.velocity_publisher.publish(vel_msg)
        # If we press control + C, the node will stop.

    def rotate(self,target_deg,func):
        command =Twist()
        stop = Twist()
        stop.linear.x = 0
        stop.angular.z = 0
        while(True):
            command.angular.z = -0.4
            func(2)
            time.sleep(0.1)
            func(3)
            time.sleep(1)
            print("target={%F} current:{%f}", target_deg,math.degrees(self.pose.theta))
            if(target_deg-self.rotate_tolerance <= math.degrees(self.pose.theta) <= target_deg+self.rotate_tolerance):
                func(3)
                time.sleep(1)
                print("reached")
                break
                