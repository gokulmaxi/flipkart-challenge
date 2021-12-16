#!/usr/bin/python3
import rospy
from geometry_msgs.msg import Twist
import paho.mqtt.client as paho
import json
import sys
def constrain(val, min_val, max_val):
    return min(max_val, max(min_val, val))
def sign_of(value):
    if value < 0 :
        return -1
    else:
        return 1
def translate(value, leftMin, leftMax, rightMin, rightMax):
    if(value == 0):
        return 0
    # Figure out how 'wide' each range is
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    # Convert the left range into a 0-1 range (float)
    valueScaled = float(value - leftMin) / float(leftSpan)

    # Convert the 0-1 range into a value in the right range.
    return rightMin + (valueScaled * rightSpan)
def omniwheel_velocity(l_x,l_y,a_z):
    rospy.loginfo("velo")
    wheel_radius = 0.28
    w_1 = (l_y/wheel_radius) + a_z
    w_2 = (l_x/wheel_radius) + a_z
    w_3 = (l_y/wheel_radius) - a_z
    w_4 = (l_x/wheel_radius) - a_z
    w_1p =constrain(translate(abs(w_1),0,2,100,190),0,255)* sign_of(w_1)
    w_2p =constrain(translate(abs(w_2),0,2,180,190),0,255)* sign_of(w_2)
    w_3p =constrain(translate(abs(w_3),0,2,180,190),0,255)* sign_of(w_3)
    w_4p =constrain(translate(abs(w_4),0,2,180,190),0,255)* sign_of(w_4)
    return [w_1p,w_2p,w_3p,w_4p]


def on_publish(client, userdata, mid):
    print("mid: "+str(mid))
    # linear velocity l_x and l_y
    # angular velocity a_z
def callback(data,args):
    bot_index = args
    velocity_list = omniwheel_velocity(data.linear.x,data.linear.y,data.angular.z)
    jsonData = {"pwm1": velocity_list[0],"pwm2":velocity_list[1],"pwm3":velocity_list[2],"pwm4":velocity_list[3]}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish("flipkart/bot"+str(bot_index), jsonEncoded, qos=1)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s and published to %d", jsonEncoded,bot_index)


def listener(bot_no):

    rospy.init_node('listener_mqtt')
    rospy.loginfo(bot_no)
    rospy.Subscriber("/flipbot"+str(bot_no)+"/cmd_vel", Twist, callback,bot_no)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


client = paho.Client()
client.on_publish = on_publish
client.connect("192.168.0.172", 1883)
client.loop_start()
if __name__ == '__main__':
    print("\nName of the bot = flibot", sys.argv[1])
    listener(int(sys.argv[1]))
