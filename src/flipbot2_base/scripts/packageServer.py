#!/usr/bin/python3
import rospy
from geometry_msgs.msg import Twist
import sys
import time
import pandas as pd
from flipbot2_msg.srv import PackageDetail,PackageDetailResponse
def request_handler(req):
    res = PackageDetailResponse()
    if(req.bot_induct == 1):
        data_tup = next(induct1_iterator)
        res.destination = dest_dict.get(data_tup[2])
        res.destString = data_tup[2]
        res.packageId = data_tup[1]
    if(req.bot_induct == 2):
        data_tup = next(induct2_iterator)
        res.destination = dest_dict.get(data_tup[2])
        res.destString = data_tup[2]
        res.packageId = data_tup[1]
    print(res.destString)
    return res
def package_server():
    rospy.init_node('package_server', anonymous=True)
    rate = rospy.Rate(10)
    server = rospy.Service('package_server', PackageDetail, request_handler)
    rospy.spin()
    # for index,rows in data.iterrows():
    #     print(rows[1])
    #     print(rows[0])

#TODO add a service server to iterators

if __name__ == '__main__':
    try:
        dest_dict = {'Pune':1,"Ahmedabad":2,'Jaipur':3,'Chennai':4,'Bengaluru':5,'Hyderabad':6,'Mumbai':7,'Delhi':8,'Kolkata':9}
        induct1_data = pd.read_csv('/home/gokul/projects/ros/flipkart-challenge/src/flipbot2_base/data/data.csv',usecols = ['induct1_shipment','induct1_package'])
        induct2_data = pd.read_csv('/home/gokul/projects/ros/flipkart-challenge/src/flipbot2_base/data/data.csv',usecols = ['induct2_shipment','induct2_package'])
        induct1_iterator= induct1_data.itertuples()
        induct2_iterator= induct2_data.itertuples()
        package_server()
    except rospy.ROSInterruptException:
        pass
