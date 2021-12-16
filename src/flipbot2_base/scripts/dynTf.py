#!/usr/bin/python3
import rospy
import sys
import tf
import tf2_ros
import geometry_msgs.msg
import math
if __name__ == '__main__':
    rospy.init_node('my_static_tf2_broadcaster')
    broadcaster = tf2_ros.TransformBroadcaster()
    static_transformStamped = geometry_msgs.msg.TransformStamped()

    tfBuffer = tf2_ros.Buffer()
    listener = tf2_ros.TransformListener(tfBuffer)
    rate = rospy.Rate(1)
    while not rospy.is_shutdown():
        while True:
            try:
                trans = tfBuffer.lookup_transform('marker_id3', 'usb_cam', rospy.Time())
                break
            except (tf2_ros.LookupException, tf2_ros.ConnectivityException, tf2_ros.ExtrapolationException):
                rate.sleep()

        static_transformStamped.header.stamp = rospy.Time.now()
        static_transformStamped.header.frame_id = "world"
        static_transformStamped.child_frame_id =  "usb_cam"
        print("PUBLISH",trans.transform.translation.y)
        static_transformStamped.transform.translation.x =  trans.transform.translation.x  - 0.1
        static_transformStamped.transform.translation.y = trans.transform.translation.y - 0.3
        static_transformStamped.transform.translation.z = trans.transform.translation.z  + 0.1
        static_transformStamped.transform.rotation.x = 1
        static_transformStamped.transform.rotation.y = 0
        static_transformStamped.transform.rotation.z = 0
        static_transformStamped.transform.rotation.w = 0

        broadcaster.sendTransform(static_transformStamped)
        rate.sleep()
