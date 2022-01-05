#!/usr/bin/python3
import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import String
import paho.mqtt.client as paho
import json
import sys
def constrain(val, min_val, max_val):
    return min(max_val, max(min_val, val))
def sign_of(value):
    if value < 0 :
        return -1
    if value == 0:
        return 0
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

    w_1p = -1 * sign_of(a_z)
    w_2p = 1 * sign_of(l_x)
    w_3p = 1 * sign_of(l_y)
    return [w_1p,w_2p,w_3p]
def on_publish(client, userdata, mid):
    print("mid: "+str(mid))
    # linear velocity l_x and l_y
    # angular velocity a_z
def callback(data,args):
    bot_index = args
    velocity_list = omniwheel_velocity(data.linear.x,data.linear.y,data.angular.z)
    print("")
    jsonData = {"angular": velocity_list[0],"linear_x":velocity_list[1],"linear_y":velocity_list[2],"servo":0}#,"pwm3":velocity_list[2],"pwm4":velocity_list[3]}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish("flipkart/bot"+str(bot_index), jsonEncoded, qos=0)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s and published to %d", jsonData,bot_index)
    if(data.angular.z != 0):
        rospy.logwarn(data.angular.z)
def callback_servo(data,args):
    bot_index = args
    jsonData = {"angular":0,"linear_x":0,"linear_y":0,"servo":data.data}
    jsonEncoded = json.dumps(jsonData)
    (rc, mid) = client.publish("flipkart/bot"+str(bot_index), jsonEncoded, qos=0)
    rospy.loginfo(rospy.get_caller_id() + "I heard %s and published to %d", jsonEncoded,bot_index)

def callback_color(data,args):
    bot_index = args
    # jsonData = {"angular":0,"linear_x":0,"linear_y":0,"servo":0,"colorRequest":1}
    # jsonEncoded = json.dumps(jsonData)
    # (rc, mid) = client.publish("flipkart/bot"+str(bot_index), jsonEncoded, qos=0)
    # rospy.loginfo(rospy.get_caller_id() + "I heard %s and published to %d", jsonEncoded,bot_index)
def listener(bot_no):

    rospy.init_node('listener_mqtt')
    rospy.loginfo(bot_no)
    rospy.Subscriber("/flipbot"+str(bot_no)+"/cmd_vel", Twist, callback,bot_no)
    rospy.Subscriber("/flipbot"+str(bot_no)+"/servo", String, callback_servo,bot_no)
    rospy.Subscriber("/flipbot"+str(bot_no)+"/colorReq", String, callback_color,bot_no)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


client = paho.Client()
client.on_publish = on_publish
client.connect("192.168.0.172", 1883)
client.loop_start()
if __name__ == '__main__':
    print("\nName of the bot = flibot", sys.argv[1])
    listener(int(sys.argv[1]))
