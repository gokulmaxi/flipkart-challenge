#!/usr/bin/python3

import math
from math import sin, cos, pi
from rospy.core import rospywarn

import tf2_ros
import rospy
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3,TransformStamped
from tf.transformations import euler_from_quaternion,quaternion_from_euler

if __name__ == '__main__':
    rospy.init_node('odometry_publisher')

    r = rospy.Rate(1.0)
    odom_pub = rospy.Publisher("robot1/odom", Odometry, queue_size=50)
    odom_broadcaster = tf2_ros.TransformBroadcaster()
    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)
    x = 0.0
    y = 0.0
    th = 0.0

    vx = 0.1
    vy = -0.1
    vth = 0.1

    current_time = rospy.Time.now()
    last_time = rospy.Time.now()
    while not rospy.is_shutdown():
        current_time = rospy.Time.now()
        rate = rospy.Rate(50.0)
        print("hello")
        try:
            trans = tfBuffer.lookup_transform(
                    'usb_cam', 'marker_id1', rospy.Time())
        except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException):
            r.sleep()
            continue
        # compute odometry from usb cam
        q = [trans.transform.rotation.x, trans.transform.rotation.y,
                trans.transform.rotation.z, trans.transform.rotation.w]
        roll, pitch, yaw = euler_from_quaternion(q)
        x = trans.transform.translation.x

        y = trans.transform.translation.y
        th = yaw
        print("hello 2")
        # since all odometry is 6DOF we'll need a quaternion created from yaw
        odom_quat = quaternion_from_euler(0, 0, th)

        t =TransformStamped()

        t.header.stamp = rospy.Time.now()
        t.header.frame_id = "robot1/odom"
        t.child_frame_id = "robot1/dummy"
        t.transform.translation.x = x
        t.transform.translation.y = y
        t.transform.translation.z = 0.0
        q = quaternion_from_euler(0, 0, th)
        t.transform.rotation.x = q[0]
        t.transform.rotation.y = q[1]
        t.transform.rotation.z = q[2]
        t.transform.rotation.w = q[3]
        odom_broadcaster.sendTransform(t)
        # next, we'll publish the odometry message over ROS
        odom = Odometry()
        odom.header.stamp = current_time
        odom.header.frame_id = "robot1/odom"

        # set the position
        odom.pose.pose = Pose(Point(x, y, 0.), Quaternion(*odom_quat))

        # set the velocity
        odom.child_frame_id = "robot1/dummy"
        odom.twist.twist = Twist(Vector3(0, 0, 0), Vector3(0, 0, 0))

        # publish the message
        odom_pub.publish(odom)
        last_time = current_time
        r.sleep()
