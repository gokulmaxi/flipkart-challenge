#!/usr/bin/python3
from inspect import trace
import string
import rospy
import paho.mqtt.client as paho
import json
from flipBot import flipbot

def on_publish(client, userdata, mid):
    print("mid: "+str(mid))


def servo_control(data , topic):
    jsonData = {"data": [0, 0, data, 0]}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish(topic, jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", jsonEncoded)


def rotate(data, topic):
    jsonData = {"data": [0, 0, 0, data]}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish(topic, jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", jsonEncoded)


client = paho.Client()
client.on_publish = on_publish
client.connect("10.10.174.148", 1883)
client.loop_start()
if __name__ == '__main__':
    try:
        rospy.init_node('bot_goal_move', anonymous=True)
        x = flipbot.FlipBot(1)
        y = flipbot.FlipBot(2)
        
        # x.move_y(0.46,-0.75)
        # x.rotate()
        # x.move_x(1.5,-0.75)
        # y.move_y()
        servo_control(1,"flipkart/bot1")
    except rospy.ROSInterruptException:
        pass
