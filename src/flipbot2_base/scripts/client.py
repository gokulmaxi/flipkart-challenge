#! /usr/bin/python3

import json
import rospy
import actionlib
from flipbot2_base.msg import BotGoalAction
from flipbot2_base.msg import BotGoalFeedback
from flipbot2_base.msg import BotGoalResult
from flipbot2_base.msg import BotGoalGoal
from rospy.timer import sleep
from std_msgs.msg import Int64
from std_msgs.msg import String
right_one_index = {1, 2, 3}
left_two_index = {7, 8, 9}


class actionclient:

    def __init__(self):
        self.client = actionlib.SimpleActionClient('bot1', BotGoalAction)
        self.pub_servo = rospy.Publisher('flipbot1/servo1', String, queue_size=10)
        self.pub_colorReq= rospy.Publisher('flipbot1/colorReq1', String, queue_size=10)
        rospy.Subscriber("chatter", Int64, self.callback)
        self.client.wait_for_server()
        self.callbackCalled = False
        self.feedback = BotGoalFeedback()
    def callback(self,data:Int64):
        self.callbackCalled = True
        print("Sending Goal")
        self.goal = BotGoalGoal(index=int(data.data))
        self.client.send_goal(self.goal)
        self.client.wait_for_result()
        self.result= self.client.get_result()
        print("got result")
        sleep(0.5)
        self.servopush = self.servodir()
        print("Actuating servo in %i",self.servopush)
        self.pub_servo.publish(str(self.servopush))
        sleep(0.5)
        self.client.send_goal(BotGoalGoal(index=-1 * int(data.data)))
        self.client.wait_for_result()
        self.callbackCalled = False
    def clientRoutine(self):
        while not rospy.is_shutdown():
            while not self.callbackCalled:
                print("waiting for color data")
                self.pub_colorReq.publish("1")
                rospy.sleep(3)
    def servodir(self):
        print(self.result)
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
    print("init node")
    ac = actionclient()
    print("starting sub")
    print("starting client routine")
    ac.clientRoutine()
    rospy.spin()
