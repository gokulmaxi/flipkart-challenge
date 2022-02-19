#! /usr/bin/python3

import rospy
import actionlib
from flipbot2_msg.msg import BotGoalAction
from flipbot2_msg.msg import BotGoalResult
from flipbot2_msg.msg import BotGoalGoal
from flipbot2_msg.srv import *
from rospy.timer import sleep
from std_msgs.msg import Int64
from std_msgs.msg import String
from geometry_msgs.msg import Twist
import sys
right_one_index = {1, 4, 7}
left_two_index = {2, 3, 5, 6, 8, 9}


class actionclient:
    def __init__(self, bot_no):
        self.client = actionlib.SimpleActionClient("bot1", BotGoalAction)
        self.bot_no = bot_no
        self.pub_servo = rospy.Publisher('servo', String, queue_size=10)
        self.pub_colorReq = rospy.Publisher('colorReq', Int64, queue_size=0)
        self.pub_cmd = rospy.Publisher('cmd_vel', Twist, queue_size=10)
        self.result = BotGoalResult()
        self.cmd_vel = Twist()
        self.stop = Twist()
        rospy.Subscriber("dest", Int64, self.callback)
        self.client.wait_for_server()
        self.callbackCalled = False

    def callback(self, data: Int64):
        self.callbackCalled = True
        rospy.loginfo("Sending Goal")
        self.goal = BotGoalGoal(index=int(data.data))
        self.client.send_goal(self.goal)
        self.client.wait_for_result()
        self.result = self.client.get_result()
        rospy.loginfo("got result")
        sleep(0.5)
        self.servopush = self.servodir()
        rospy.loginfo("Actuating servo in %i", self.servopush)
        self.pub_servo.publish(str(self.servopush))
        sleep(1.5)
        self.cmd_vel.linear.x = self.servopush
        self.pub_cmd.publish(self.cmd_vel)
        sleep(1.0)
        self.pub_cmd.publish(self.stop)
        self.client.send_goal(BotGoalGoal(index=-1 * int(data.data)))
        self.client.wait_for_result()
        self.result = self.client.get_result()
        self.callbackCalled = False
        sleep(1.0)

    def clientRoutine(self):
        while not rospy.is_shutdown():
            control_bit = rospy.g("bot_control")
            if(control_bit == 1):
                while not self.callbackCalled:
                    rospy.loginfo("waiting for color data")
                    if(self.result.inductIndex != 0):
                        self.pub_colorReq.publish(self.result.inductIndex)
                        rospy.sleep(3)
                    else:
                        print("Initating")
                        try:
                            trans = self.tfBuffer.lookup_transform(
                                'world', 'marker_id'+str(self.bot_no), rospy.Time())
                        except (self.tf2_ros.LookupException, self.tf2_ros.ConnectivityException, self.tf2_ros.ExtrapolationException):
                            self.rate.sleep()
                            continue
                        # Find Induct and publish
                        if(abs(trans.transform.translation.x - 1.018) < 0.3):
                            self.pub_colorReq.publish(1)
                        elif(abs(trans.transform.translation.y - 1.773) < 0.3):
                            self.pub_colorReq.publish(2)

    def servodir(self):
        if self.result.destIndex in right_one_index:
            return 1
        else:
            return -1
        if self.result.destIndex in left_two_index:
            return 1
        else:
            return -1


if __name__ == "__main__":
    rospy.init_node('client'+str(sys.argv[1]))
    rospy.loginfo("Init client node " + sys.argv[1])
    ac = actionclient(int(sys.argv[1]))
    ac.clientRoutine()
    rospy.spin()
