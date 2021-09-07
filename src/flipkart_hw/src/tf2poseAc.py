#!/usr/bin/env python3
import rospy

import math
import tf2_ros
from geometry_msgs.msg import Pose
from tf.transformations import euler_from_quaternion

if __name__ == '__main__':
    rospy.init_node('flipbot_tf_listener')

    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)

    turtle_pose = rospy.Publisher('/turtle1/pose', Pose, queue_size=1)

    rate = rospy.Rate(10.0)
    while not rospy.is_shutdown():
        try:
            trans = tfBuffer.lookup_transform( 'usb_cam','marker_id1', rospy.Time())
        except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException):
            rate.sleep()
            continue

        msg = Pose()
        msg.position.x=trans.transform.translation.x
        msg.position.y=trans.transform.translation.y
        msg.position.z=trans.transform.translation.z
        msg.orientation.x=trans.transform.rotation.x
        msg.orientation.z=trans.transform.rotation.y
        msg.orientation.y=trans.transform.rotation.z
        msg.orientation.w=trans.transform.rotation.w
        turtle_pose.publish(msg)

        rate.sleep()
