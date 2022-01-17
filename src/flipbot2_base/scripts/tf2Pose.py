#!/usr/bin/env python3
import rospy

import math
import tf2_ros
from turtlesim.msg import Pose
from tf.transformations import euler_from_quaternion
import sys

if __name__ == '__main__':
    rospy.init_node('flipbot_tf_listener_1')
    rospy.loginfo("initiating transform look up for bot : " + str(sys.argv[1]))
    bot_id = "marker_id"+sys.argv[1]
    bot_publisher = "flipbot"+str(sys.argv[1])+"/pose"
    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)

    turtle_pose = rospy.Publisher(bot_publisher, Pose, queue_size=1)

    rate = rospy.Rate(50.0)
    while not rospy.is_shutdown():
        try:
            trans = tfBuffer.lookup_transform(
                'world',bot_id, rospy.Time())
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
