# flipkart grid challenge

## Custom Map 
![gazebo custom map](./assets/gazebo-custom-map.png) 

## Intel realsense gazebo plugin 
```bash
git submodule add git submodule add https://github.com/issaiass/realsense2_description
git submodule add https://github.com/issaiass/realsense_gazebo_plugin
```
- steps to convert xacro to urdf
```
rosrun xacro xacro test_d435_camera.urdf.xacro > d435.urdf  # convert xacro to urdf
gz sdf -p d435.urdf > d435.sdf         # urdf to sdf
```
- detailed explaination [here](https://nu-msr.github.io/me495_site/lecture10_sdf_gazebo.html) 

## Group launch of robots
[multiple controller error](https://robotics.stackexchange.com/questions/21673/tried-to-advertise-a-service-that-is-already-advertised-in-this-node-leo-contr) 

##TODO
- add coordinates to spawn 
- add opencv to detect