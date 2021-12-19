#!/usr/bin/python3
import rospy
from geometry_msgs.msg import Twist
import sys
import time
def turtle_circle():
    rospy.init_node('turtlesim', anonymous=True)
    pub = rospy.Publisher('/flipbot1/cmd_vel',
                          Twist, queue_size=10)
    rate = rospy.Rate(10)
    vel = Twist()
    while not rospy.is_shutdown():
        vel.linear.x = 0
        vel.linear.y = 0
        vel.linear.z = 0
        vel.angular.x = 0
        vel.angular.y = 0
        vel.angular.z = 2
        rospy.loginfo("Radius = "
                      )
        pub.publish(vel)
        time.sleep(0.2)
        vel.angular.z = 0
        pub.publish(vel)
        time.sleep(1)
        # rate.sleep()


if __name__ == '__main__':
    try:
        turtle_circle()
    except rospy.ROSInterruptException:
        pass
