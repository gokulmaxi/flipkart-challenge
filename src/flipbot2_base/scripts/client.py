#! /usr/bin/python3

import flipbot2_base
import rospy
import actionlib
from flipbot2_base.msg import BotGoalAction
from flipbot2_base.msg import BotGoalFeedback
from flipbot2_base.msg import BotGoalResult
from flipbot2_base.msg import BotGoalGoal
import paho.mqtt.client as mqtt

client = actionlib.SimpleActionClient('bot1', BotGoalAction)


def callback(c, i, msg):
    goal = BotGoalGoal(index=int(msg.payload))
    print(int(msg.payload))
    client.send_goal(goal)
    client.wait_for_result()


if __name__ == "__main__":

    try:

        rospy.init_node('client')

        client_ip = "192.168.0.172"

        mqtt_client = mqtt.Client()

        mqtt_client.on_message = callback

        mqtt_client.connect(client_ip)

        mqtt_client.subscribe("flipkart/bot1")

        client.wait_for_server()

        mqtt_client.loop_forever()

    except rospy.ROSInterruptException:

        pass
