#!/usr/bin/python3

import math
from math import sin, cos, pi

import tf2_ros
import rospy
import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3
from tf.transformations import euler_from_quaternion,quaternion_from_euler
rospy.init_node('odometry_publisher')

odom_pub = rospy.Publisher("robot1/odom", Odometry, queue_size=50)
odom_broadcaster = tf.TransformBroadcaster()

x = 0.0
y = 0.0
th = 0.0

vx = 0.1
vy = -0.1
vth = 0.1

current_time = rospy.Time.now()
last_time = rospy.Time.now()

r = rospy.Rate(1.0)
while not rospy.is_shutdown():
    current_time = rospy.Time.now()
    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)
    rate = rospy.Rate(50.0)
    try:
       trans = tfBuffer.lookup_transform(
                'usb_cam', 'marker_id1', rospy.Time())
    except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException):
         rate.sleep()
         continue
    # compute odometry from usb cam
    q = [trans.transform.rotation.x, trans.transform.rotation.y,
             trans.transform.rotation.z, trans.transform.rotation.w]
    roll, pitch, yaw = euler_from_quaternion(q)
    x = trans.transform.translation.x

    y = trans.transform.translation.y
    th = yaw

    # since all odometry is 6DOF we'll need a quaternion created from yaw
    odom_quat = quaternion_from_euler(0, 0, th)

    # first, we'll publish the transform over tf
    odom_broadcaster.sendTransform(
        (x, y, 0.),
        odom_quat,
        current_time,
        "robot1/dummy",
        "odom"
    )

    # next, we'll publish the odometry message over ROS
    odom = Odometry()
    odom.header.stamp = current_time
    odom.header.frame_id = "odom"

    # set the position
    odom.pose.pose = Pose(Point(x, y, 0.), Quaternion(*odom_quat))

    # set the velocity
    odom.child_frame_id = "robot1/dummy"
    odom.twist.twist = Twist(Vector3(0, 0, 0), Vector3(0, 0, 0))

    # publish the message
    odom_pub.publish(odom)

    last_time = current_time
    r.sleep()
