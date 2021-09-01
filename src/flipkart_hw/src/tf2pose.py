#!/usr/bin/env python3
import rospy

import math
import tf2_ros
import geometry_msgs.msg
from turtlesim.msg import Pose
import turtlesim.srv
from tf.transformations import euler_from_quaternion

if __name__ == '__main__':
    rospy.init_node('tf2_turtle_listener')

    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)

    turtle_pose = rospy.Publisher('/turtle1/pose', turtlesim.msg.Pose, queue_size=1)

    rate = rospy.Rate(10.0)
    while not rospy.is_shutdown():
        try:
            trans = tfBuffer.lookup_transform('marker_id1', 'usb_cam', rospy.Time())
        except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException):
            rate.sleep()
            continue

        msg = turtlesim.msg.Pose()
        
        # msg.angular.z = 4 * math.atan2(trans.transform.translation.y, trans.transform.translation.x)
        # msg.linear.x = 0.5 * math.sqrt(trans.transform.translation.x ** 2 + trans.transform.translation.y ** 2)
        msg.x=trans.transform.translation.x
        msg.y=trans.transform.translation.y
        msg.theta= euler_from_quaternion([trans.transform.rotation.x,trans.transform.rotation.y,trans.transform.rotation.z,trans.transform.rotation.w])[2]
        turtle_pose.publish(msg)

        rate.sleep()
