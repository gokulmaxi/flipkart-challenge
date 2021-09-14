#!/usr/bin/env python
from inspect import trace
import string
import rospy
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose
import paho.mqtt.client as paho
import json
from flipBot import flipbot

def on_publish(client, userdata, mid):
    print("mid: "+str(mid))


def servo_control(data: int, topic: string):
    jsonData = {"data": [0, 0, data, 0]}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish("test/message", jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", jsonEncoded)


def rotate(data: int, topic: string):
    jsonData = {"data": [0, 0, 0, data]}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish("test/message", jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", jsonEncoded)


client = paho.Client()
client.on_publish = on_publish
client.connect("10.10.174.148", 1883)
client.loop_start()
if __name__ == '__main__':
    try:
        rospy.init_node('bot_goal_move', anonymous=True)
        x = flipbot.FlipBot(1)
        x.move_y()
        x.rotate()
        x.move_x(1.35,-0.6)
        # servo_control(1,"test/message")
    except rospy.ROSInterruptException:
        pass
