#!/usr/bin/python3
import rospy
from std_msgs.msg import Int64
from std_msgs.msg import String
from flipbot2_msg.srv import *
import paho.mqtt.client as mqtt
import sys
class color_bridge:
    def __init__(self,bot_no,method) -> None:
        self.pub = rospy.Publisher('dest', Int64, queue_size=10)
        self.rate = rospy.Rate(10) # 10hz
        self.bot_no = bot_no
        if(method == 2):  #method 2 - mqtt color res from esp32
            self.client_ip = "192.168.0.172"
            self.mqtt_client = mqtt.Client()
            self.mqtt_client.connect(self.client_ip)
            self.mqtt_client.subscribe("flipkart/color"+str(bot_no))
            self.mqtt_client.on_message = self.callback
            self.mqtt_client.loop_forever()
        if(method == 1):
            print("method 1")
            rospy.wait_for_service('/package_server')
            rospy.Subscriber("colorReq",Int64,self.callbackRos)
    def callback(self, c, i, msg):
        self.pub.publish(int(msg.payload))
    def callbackRos(self,data):
        try:
            print("calling package server")
            packageRequest= rospy.ServiceProxy('/package_server', PackageDetail)
            resp1 = packageRequest(1,1)
            self.pub.publish(resp1.destination)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)

if __name__ == '__main__':
         rospy.init_node('color_bridge', anonymous=True)
         print(sys.argv[2])
         mqtt_bridge = color_bridge(sys.argv[1],1)
         rospy.spin()

