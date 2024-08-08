#ifndef global_data_H
#define global_data_H
#include "std_library.h"

class server_global_data {
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


#endif global_data_H