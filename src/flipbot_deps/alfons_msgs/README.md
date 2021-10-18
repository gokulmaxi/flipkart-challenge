# alfons_msgs

## Overview

This is a [ROS] package developed for the [alfons robot](https:://simact.de/robots/alfons/index.html). It contains different messages for this project. 
The Robot-Centric Elevation Mapping packages have been tested under ROS Kinetic and Ubuntu 16.04. This is research code, expect that it changes often and any fitness for a particular purpose is disclaimed.

**Author: Markus Lamprecht<br />
Maintainer: Markus Lamprecht, 2f4yor@gmail.com<br />**

## Installation

### Dependencies

This software is built on the Robotic Operating System ([ROS]), which needs to be [installed](http://wiki.ros.org) first. Additionally, it depends on

- geometry_msgs
- std_msgs


### Building

In order to install the Robot-Centric Elevation Mapping, clone the latest version from this repository into your catkin workspace and compile the package using ROS.

    cd catkin_workspace/src
    git clone https://github.com/cesmak/alfons_msgs.git
    cd ../
    catkin build