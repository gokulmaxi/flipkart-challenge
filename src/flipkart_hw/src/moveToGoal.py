#!/usr/bin/python3
from inspect import trace
import string
import time
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


def rotate(data:int):
    jsonData = {"data": [0, 0, 0, data]}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish("flipkart/bot1", jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", jsonEncoded)


client = paho.Client()
client.on_publish = on_publish
client.connect("10.10.174.148", 1883)
client.loop_start()
if __name__ == '__main__':
    try:
        rospy.init_node('bot_goal_move', anonymous=True)
        bot1 = flipbot.FlipBot(1)
        bot2 = flipbot.FlipBot(2)
        bot1.move_y(0.42,-0.45,True)
        bot1.rotate(85,rotate)
        bot1.move_x(1.3,-0.55,True)
        time.sleep(0.5)
        servo_control(1,"flipkart/bot1")
        time.sleep(0.5)
        bot1.move_x(0.42,-0.55,False)
        bot1.rotate(0,rotate)
        bot1.move_y(0.42,0.65,False)
        # bot2.move_y(0.28,-0.75)
    except rospy.ROSInterruptException:
        pass
