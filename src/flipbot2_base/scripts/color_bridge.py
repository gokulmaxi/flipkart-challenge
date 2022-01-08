#!/usr/bin/python3
import rospy
from std_msgs.msg import Int64
import paho.mqtt.client as mqtt
import sys
class color_bridge:
    def __init__(self,bot_no) -> None:
        self.pub = rospy.Publisher('dest', Int64, queue_size=10)
        self.rate = rospy.Rate(10) # 10hz
        self.client_ip = "192.168.0.172"
        self.mqtt_client = mqtt.Client()
        self.mqtt_client.connect(self.client_ip)
        self.mqtt_client.subscribe("flipkart/color"+str(bot_no))
        self.mqtt_client.on_message = self.callback
        self.mqtt_client.loop_forever()
    def callback(self, c, i, msg):
        self.pub.publish(int(msg.payload))

if __name__ == '__main__':
         rospy.init_node('color_bridge', anonymous=True)
         mqtt_bridge = color_bridge(sys.argv[1])
