#!/usr/bin/python3
import rospy
from std_msgs.msg import Int64
import paho.mqtt.client as mqtt
class color_bridge:
    def __init__(self) -> None:
        self.pub = rospy.Publisher('chatter', Int64, queue_size=10)
        self.rate = rospy.Rate(10) # 10hz
        self.client_ip = "192.168.0.172"
        self.mqtt_client = mqtt.Client()
        self.mqtt_client.connect(self.client_ip)
        self.mqtt_client.subscribe("flipkart/color1")
        self.mqtt_client.on_message = self.callback
        self.mqtt_client.loop_forever()
    def callback(self, c, i, msg):
        print("executing callback")
        self.pub.publish(int(msg.payload))

if __name__ == '__main__':
         rospy.init_node('color_bridge', anonymous=True)
         mqtt_bridge = color_bridge()
