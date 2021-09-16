#!/usr/bin/env python3
import rospy

import math
import tf2_ros
from turtlesim.msg import Pose
from tf.transformations import euler_from_quaternion

if __name__ == '__main__':
    rospy.init_node('flipbot_tf_listener_3')

    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)

    turtle_pose = rospy.Publisher('/flipbot3/pose', Pose, queue_size=1)

    rate = rospy.Rate(50.0)
    while not rospy.is_shutdown():
        try:
            trans = tfBuffer.lookup_transform(
                'usb_cam', 'marker_id3', rospy.Time())
        except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException):
            rate.sleep()
            continue

        msg = Pose()
        q = [trans.transform.rotation.x, trans.transform.rotation.y,
             trans.transform.rotation.z, trans.transform.rotation.w]
        roll, pitch, yaw = euler_from_quaternion(q)
        msg.x = trans.transform.translation.x
        msg.y = trans.transform.translation.y
        msg.theta=yaw
        turtle_pose.publish(msg)

        rate.sleep()
