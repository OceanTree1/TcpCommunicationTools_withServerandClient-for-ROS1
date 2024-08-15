#ifndef std_library_H
#define std_library_H

#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <nav_msgs/Odometry.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <tf/transform_datatypes.h>
#include <std_msgs/Float32MultiArray.h>
#include <cmath>

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <mutex>
#include <algorithm>

#include <vector>
#include <tf/tf.h>
#include <thread>
#include <condition_variable>

#include <bits/stdc++.h>
#include <ctime> 
#include <fcntl.h>

// 全局变量
class server_share_data{
public:
    // 全局报文头部与尾部
    static const std::vector<uint8_t> Recv_head;      // 头部
    static const std::vector<uint8_t> Recv_end;       // 尾部

    // 全局原子标志位
    static std::atomic<bool> running_Main;
    static std::atomic<bool> running_Modbus;

    // 全局反馈数据
    static std::vector<uint8_t> Information_Feedback;  // 反馈数据举例
};

#endif