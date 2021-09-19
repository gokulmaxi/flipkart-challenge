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
tolerance=[1.5,2.5,2,2]
#* STARTING TO L JOINT
move_y_f = [pair(0.42,-0.61),pair(0.3,-0.485),pair(0.27,-0.385),pair(0.27,-0.48)]
rotate_f1 = [86,95,-90,-88]
move_x_f1 = [pair(1.2,-0.61),pair(0.7,-0.65),pair(-0.50,-0.50),pair(-0.5,-0.50)]
rotate_f2 = [8,90,-90,-88]
move_x_f2 = [pair(1.62,-0.61),pair(1.33,-0.66),pair(-0.91,-0.50),pair(-0.91,-0.50)]
move_x_b = [pair(0.43,-0.62),pair(0.557,-0.67),pair(0.05,-0.50),pair(-0.25,0.50)]
#rotate_b = [-2,0.4,0,2]
rotate_b1 = [-1.0,0.8,0.3,1]
move_y_b1 = [pair(0.42,0.0),pair(0.43,0.20),pair(0.43,0.04),pair(0.43,-0.2)]
rotate_b2 = [-8,-1,0.3,6]
move_y_b2 = [pair(0.42,0.48),pair(0.43,0.36),pair(0.43,0.31),pair(0.43,0.15)]
rotate_b3 = [-9,-6,0.3,11]
#* END POINT
move_y_b3 = [pair(0.43,0.79),pair(0.43,0.67),pair(0.43,0.67),pair(0.43,0.67)]

if __name__ == '__main__':
    try:
        rospy.init_node('bot_goal_move', anonymous=True)
        bots = [flipbot.FlipBot(1),flipbot.FlipBot(2),flipbot.FlipBot(3),flipbot.FlipBot(4)]
        time.sleep(0.5)
        for index in range(0,4):
            bots[index].rotate_tolerance=tolerance[index]
            rospy.loginfo("executing bot %d",index+1)
            bots[index].move_y(move_y_f[index],True)
            time.sleep(0.5)
            bots[index].rotate(rotate_f1[index],rotate,"flipkart/bot"+str(index+1))
            time.sleep(0.3)
            if index != 0:
                bots[index].move_x(move_x_f1[index],True)
                time.sleep(0.3)
                bots[index].rotate(rotate_f2[index],rotate,"flipkart/bot"+str(index+1))
                time.sleep(0.5)            
            bots[index].move_x(move_x_f2[index],True)
            time.sleep(0.5)
            servo_control(1,"flipkart/bot"+str(index+1))
            time.sleep(0.5)
            bots[index].move_x(move_x_b[index],False)
            time.sleep(0.5)
            bots[index].rotate(rotate_b1[index],rotate,"flipkart/bot"+str(index+1))
            time.sleep(0.5)
            bots[index].move_y(move_y_b1[index],False)
            time.sleep(0.5)
            bots[index].rotate(rotate_b2[index],rotate,"flipkart/bot"+str(index+1))
            time.sleep(0.5)
            bots[index].move_y(move_y_b2[index],False)
            time.sleep(0.5)
            bots[index].rotate(rotate_b3[index],rotate,"flipkart/bot"+str(index+1))
            time.sleep(0.5)
            bots[index].move_y(move_y_b3[index],False)

 
    except rospy.ROSInterruptException:
        pass
