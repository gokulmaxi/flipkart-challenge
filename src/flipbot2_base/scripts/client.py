#! /usr/bin/python3

import json
import flipbot2_base
import rospy
import actionlib
from flipbot2_base.msg import BotGoalAction
from flipbot2_base.msg import BotGoalFeedback
from flipbot2_base.msg import BotGoalResult
from flipbot2_base.msg import BotGoalGoal
import paho.mqtt.client as mqtt

right_one_index = {1, 2, 3}
left_two_index = {7, 8, 9}


class actionclient:

    def __init__(self):
        self.result = BotGoalResult()
        self.client = actionlib.SimpleActionClient('bot1', BotGoalAction)
        self.client_ip = "192.168.0.172"
        self.mqtt_client = mqtt.Client()
        self.mqtt_client.connect(self.client_ip)
        self.mqtt_client.subscribe("flipkart/color1")
        self.mqtt_client.on_message = self.callback
        self.client.wait_for_server()
        self.mqtt_client.loop_forever()

    def callback(self, c, i, msg):
        self.goal = BotGoalGoal(index=int(msg.payload))
        print(int(msg.payload))
        self.client.send_goal(self.goal)
        self.client.wait_for_result()
        self.clientResult = self.client.get_result()
        self.servopush = self.servodir()
        self.jsonData = {"angular": 0, "linear_x": 0,
                         "linear_y": 0, "servo": self.servopush}
        self.jsonEncoded = json.dumps(self.jsonData)
        self.mqtt_client.publish("flipkart/bot"+str(1), self.jsonEncoded, qos=0)
        self.client.send_goal(BotGoalGoal(index=-1 * int(msg.payload)))
        self.client.wait_for_result()

    def servodir(self):

        if(self.result.inductIndex == 1):
            if self.result.destIndex in right_one_index:
                return 1
            else:
                return -1
        if(self.result.inductIndex == 2):
            if self.result.destIndex in left_two_index:
                return 1
            else:
                return -1

if __name__ == "__main__":
    rospy.init_node('client')
    ac = actionclient()
    rospy.spin()
