#!/usr/bin/python3
import rospy
from geometry_msgs.msg import Twist
import paho.mqtt.client as paho
import json
import sys

def on_publish(client, userdata, mid):
    print("mid: "+str(mid))
    # linear velocity l_x and l_y
    # angular velocity a_z
def omniwheel_velocity(l_x,l_y,a_z):
    wheel_radius = 0.28
    w_1 = (l_y/wheel_radius) + a_z
    w_2 = (l_x/wheel_radius) + a_z
    w_3 = (l_y/wheel_radius) + a_z
    w_4 = (l_x/wheel_radius) + a_z
    return [w_1,w_2,w_3,w_4]
def callback(data,args):
    bot_index = args
    velocity_list = omniwheel_velocity(data.linear.x,data.linear.y,data.angular.z)
    jsonData = {"data": velocity_list}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish("flipkart/bot"+str(bot_index), jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s and published to %d", jsonEncoded,bot_index)


def listener(bot_no):

    rospy.init_node('listener_mqtt')
    rospy.Subscriber("/flipbot1/cmd_vel", Twist, callback,bot_no)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


client = paho.Client()
client.on_publish = on_publish
client.connect("10.10.165.7", 1883)
client.loop_start()
if __name__ == '__main__':
    print("\nName of the bot = flibot", sys.argv[1])
    listener(int(sys.argv[1]))
