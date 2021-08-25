#!/usr/bin/env python3
import rospy
from geometry_msgs.msg import Twist
import paho.mqtt.client as paho
import json
def on_publish(client, userdata, mid):
    print("mid: "+str(mid))
def callback(data):
    jsonData={"data":[data.linear.x,data.angular.z]}
    jsonEncoded=json.dumps(jsonData)
    (rc, mid) = client.publish("test/message",jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", jsonEncoded)

def listener():


    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("/turtle1/cmd_vel", Twist, callback)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


client = paho.Client()
client.on_publish = on_publish
client.connect("10.10.174.148", 1883)
client.loop_start()
if __name__ == '__main__':
    listener()
