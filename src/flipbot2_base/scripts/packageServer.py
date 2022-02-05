#!/usr/bin/python3
import rospy
from geometry_msgs.msg import Twist
import sys
import time
import pandas as pd
def package_server():
    rospy.init_node('package_server', anonymous=True)
    rate = rospy.Rate(10)
    induct1_data = pd.read_csv('/home/gokul/projects/ros/flipkart-challenge/src/flipbot2_base/data/data.csv',usecols = ['induct1_shipment','induct1_package'])
    induct2_data = pd.read_csv('/home/gokul/projects/ros/flipkart-challenge/src/flipbot2_base/data/data.csv',usecols = ['induct2_shipment','induct2_package'])
    induct1_iterator= induct1_data.itertuples()
    induct1_iterator= induct2_data.itertuples()
    # for index,rows in data.iterrows():
    #     print(rows[1])
    #     print(rows[0])

#TODO add a service server to iterators

if __name__ == '__main__':
    try:
        package_server()
    except rospy.ROSInterruptException:
        pass
