#!/usr/bin/python3
import rospy
from geometry_msgs.msg import Twist
import sys
import time
def package_server():
    rospy.init_node('package_server', anonymous=True)
    rate = rospy.Rate(10)
#TODO add a service server

if __name__ == '__main__':
    try:
        package_server()
    except rospy.ROSInterruptException:
        pass
