#!/usr/bin/env python3
import rospy
from nav_msgs.msg import Odometry
from tf.transformations import euler_from_quaternion, quaternion_from_euler
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose
import time
import math

yaw = 0.0
target = 90
kp=0.09

def get_rotation (pose):
    global yaw
    yaw = pose.theta


rospy.init_node('rotate_robot')

rospy.Subscriber('/turtle1/pose', Pose, get_rotation)
pub = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=1)
r = rospy.Rate(20)
r2 = rospy.Rate(4)
command =Twist()
stop = Twist()
stop.linear.x = 0 
stop.angular.z = 0
while not rospy.is_shutdown():
    #quat = quaternion_from_euler (roll, pitch,yaw)
    #print quat
  
    target_rad = target*math.pi/180
    command.angular.z = -0.3
    pub.publish(command)
    r.sleep()
    pub.publish(stop)
    time.sleep(1)
    print("target={} current:{}", target,math.degrees(yaw))
    if(math.degrees(yaw)<100 and math.degrees(yaw)>89):
    # and math.degrees(yaw)>80):
        pub.publish(stop)
        print("reached")
        break
    r.sleep()