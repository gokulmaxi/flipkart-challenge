#!/usr/bin/zsh
kitty -e roslaunch flipbot2_base flipkart_round2.launch  &
kitty -e roslaunch flipbot2_base collision_detector.launch &
rosrun flipbot2_base packageServer.py



