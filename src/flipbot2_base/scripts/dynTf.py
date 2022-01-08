#!/usr/bin/python3
import rospy
import sys
import tf
import tf2_ros
import geometry_msgs.msg
import math
if __name__ == '__main__':
    rospy.init_node('my_static_tf2_broadcaster')
    broadcaster = tf2_ros.StaticTransformBroadcaster()
    static_transformStamped = geometry_msgs.msg.TransformStamped()
    rate = rospy.Rate(20)
    static_transformStamped.header.stamp = rospy.Time.now()
    static_transformStamped.header.frame_id = "world"
    static_transformStamped.child_frame_id =  "usb_cam"
    static_transformStamped.transform.translation.x = -1.8
    static_transformStamped.transform.translation.y = 1.6
    static_transformStamped.transform.translation.z = 3.62
    static_transformStamped.transform.rotation.x = 1
    static_transformStamped.transform.rotation.y = 0
    static_transformStamped.transform.rotation.z = 0
    static_transformStamped.transform.rotation.w = 0
    broadcaster.sendTransform(static_transformStamped)
    rospy.spin()
