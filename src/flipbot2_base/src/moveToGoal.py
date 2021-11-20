import string
import time
import rospy
import paho.mqtt.client as paho
import json
from flipBot2 import flipbot2
from flipBot2.pair import pair
def on_publish(client, userdata, mid):
    print("mid: "+str(mid))


def servo_control(data , topic):
    jsonData = {"data": [0, 0, data, 0]}
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
        bots = [flipbot2.FlipBot(1)] # trying with one bot



    except rospy.ROSInterruptException:
        pass
