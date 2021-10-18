std_msgs/Header header
geometry_msgs/Point input_position
geometry_msgs/Point position_kf
geometry_msgs/Point velocity_kf
geometry_msgs/Point acceleration_kf


# Row-major representation of the 3x3 covariance matrix
# The orientation parameters use a fixed-axis representation.
# In order, the parameters are:
# (x, y, z, rotation about X axis, rotation about Y axis, rotation about Z axis)
# covariance of the position estimated kf not of the quaternion
float64[6] covariance_position


#geometry_msgs/Quaternion orientation_pos_kf


