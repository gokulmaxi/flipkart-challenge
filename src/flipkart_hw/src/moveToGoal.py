#!/usr/bin/python3
from inspect import trace
import string
import time
import rospy
import paho.mqtt.client as paho
import json
from flipBot import flipbot
from flipBot.pair import pair
def on_publish(client, userdata, mid):
    print("mid: "+str(mid))


def servo_control(data , topic):
    jsonData = {"data": [0, 0, data, 0]}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish(topic, jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", jsonEncoded)


def rotate(data:int,topic):
    jsonData = {"data": [0, 0, 0, data]}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish(topic, jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", jsonEncoded)


client = paho.Client()
client.on_publish = on_publish
client.connect("10.10.174.148", 1883)
client.loop_start()
#goal points for all four bots
# bot1 | bot2 | bot3 | bot4
#move_y_f = [pair(0.42,-0.62),pair(0.27,-0.48),pair(0.27,-0.39),pair(0.27,-0.51)]
move_y_f = [pair(0.42,-0.62),pair(0.27,-0.50),pair(0.27,-0.40),pair(0.27,-0.54)]
rotate_f = [85,95,-89,-88]
move_x_f = [pair(1.57,-0.62),pair(1.27,-0.65),pair(-0.98,-0.50),pair(0.9,-0.50)]
move_x_b = [pair(0.40,-0.62),pair(0.54,-0.69),pair(-0.05,-0.50),pair(-0.23,0.50)]
#rotate_b = [-2,0.5,0,2]
rotate_b = [-1.0,0.5,0,2]
move_y_b = [pair(0.42,0.68),pair(0.43,0.51),pair(0.43,0.51),pair(0.43,0.54)]

if __name__ == '__main__':
    try:
        rospy.init_node('bot_goal_move', anonymous=True)
        bots = [flipbot.FlipBot(1),flipbot.FlipBot(2),flipbot.FlipBot(3),flipbot.FlipBot(4)]
        time.sleep(0.5)
        for index in range(0,2):
            rospy.loginfo("executing bot %d",index+1)
            bots[index].move_y(move_y_f[index],True)
            time.sleep(0.5)
            bots[index].rotate(rotate_f[index],rotate,"flipkart/bot"+str(index+1))
            time.sleep(0.5)
            bots[index].move_x(move_x_f[index],True)
            time.sleep(0.5)
            servo_control(1,"flipkart/bot"+str(index+1))
            time.sleep(0.5)
            bots[index].move_x(move_x_b[index],False)
            time.sleep(0.5)
            bots[index].rotate(rotate_b[index],rotate,"flipkart/bot"+str(index+1))
            time.sleep(0.5)
            bots[index].move_y(move_y_b[index],False)
 
    except rospy.ROSInterruptException:
        pass
