#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include "IPCServerTools_v1/global_func.h"

std::vector<uint8_t> CodeLength = {0x00, 0x02};
//返回成功码拼接
bool ReturnSuccessful(int client_socket, std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, uint16_t ErrorCode, std::vector<uint8_t> Recv_head, std::vector<uint8_t> Recv_end){
    std::vector<uint8_t> Controller_send_Successful = {};       //初始化成功消息
    std::vector<uint8_t> Successful_code = server_global_functools_uintChange::uint16ToVector(ErrorCode);
    Controller_send_Successful.insert(Controller_send_Successful.end(),ID_buffer.begin(),ID_buffer.end());              // 放入自增序列
    Controller_send_Successful.insert(Controller_send_Successful.end(),Function_buffer.begin(),Function_buffer.end());  // 放入功能码
    Controller_send_Successful.insert(Controller_send_Successful.end(),CodeLength.begin(),CodeLength.end());  // 放入长度
    Controller_send_Successful.insert(Controller_send_Successful.end(),Successful_code.begin(),Successful_code.end());  // 放入成功码
    uint16_t calculatedCrc16_back = server_global_functools_Crc::CalculateCrc16(Controller_send_Successful.data(), Controller_send_Successful.size());
    std::vector<uint8_t> calculatedCrc16_back_uint8 = server_global_functools_uintChange::uint16ToVector(calculatedCrc16_back);
    Controller_send_Successful.insert(Controller_send_Successful.end(),calculatedCrc16_back_uint8.begin(),calculatedCrc16_back_uint8.end());//放入crc16校验码
    Controller_send_Successful.insert(Controller_send_Successful.begin(),Recv_head.begin(),Recv_head.end());            // 放入头部
    Controller_send_Successful.insert(Controller_send_Successful.end(),Recv_end.begin(),Recv_end.end());                // 放入尾部
    send(client_socket, Controller_send_Successful.data(), Controller_send_Successful.size(), 0);                       // 发送成功信息
    std::cout << "已顺利返回“成功”Message给上位机" << std::endl;
    return true;
}

bool ReturnFalse(int client_socket, std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, uint16_t ErrorCode, std::vector<uint8_t> Recv_head, std::vector<uint8_t> Recv_end){
    //返回上位机失败指令
    std::vector<uint8_t> False_code = server_global_functools_uintChange::uint16ToVector(ErrorCode);
    std::vector<uint8_t> Controller_send_False = {};       // 初始化失败消息
    Controller_send_False.insert(Controller_send_False.end(),ID_buffer.begin(),ID_buffer.end());                // 放入自增序列
    Controller_send_False.insert(Controller_send_False.end(),Function_buffer.begin(),Function_buffer.end());    // 放入功能码
    Controller_send_False.insert(Controller_send_False.end(),CodeLength.begin(),CodeLength.end());              // 放入长度
    Controller_send_False.insert(Controller_send_False.end(),False_code.begin(),False_code.end());              // 放入失败码
    uint16_t calculatedCrc16_back = server_global_functools_Crc::CalculateCrc16(Controller_send_False.data(), Controller_send_False.size());
    std::vector<uint8_t> calculatedCrc16_back_uint8 = server_global_functools_uintChange::uint16ToVector(calculatedCrc16_back);
    Controller_send_False.insert(Controller_send_False.end(),calculatedCrc16_back_uint8.begin(),calculatedCrc16_back_uint8.end());//放入crc16校验码
    Controller_send_False.insert(Controller_send_False.begin(),Recv_head.begin(),Recv_head.end());              // 放入头部
    Controller_send_False.insert(Controller_send_False.end(),Recv_end.begin(),Recv_end.end());                  // 放入尾部
    send(client_socket, Controller_send_False.data(), Controller_send_False.size(), 0);                         // 发送失败消息
    std::cout << "已顺利返回“失败”Message给上位机" << std::endl;
    return true;
}

bool ReturnData(int client_socket, std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, std::vector<uint8_t> BackData, std::string BackDataMode, std::vector<uint8_t> Recv_head, std::vector<uint8_t> Recv_end){
    std::vector<uint8_t> BackDataFull = {};       //初始化返回消息
    uint16_t BackData_uint16 = static_cast<uint8_t>(BackData.size());
    std::vector<uint8_t> BackDataLength = server_global_functools_uintChange::uint16ToVector(BackData_uint16);
    BackDataFull.insert(BackDataFull.end(),ID_buffer.begin(),ID_buffer.end());              // 放入自增序列
    BackDataFull.insert(BackDataFull.end(),Function_buffer.begin(),Function_buffer.end());  // 放入功能码
    BackDataFull.insert(BackDataFull.end(),BackDataLength.begin(),BackDataLength.end());    // 放入长度
    BackDataFull.insert(BackDataFull.end(),BackData.begin(),BackData.end());                // 放入数据
    uint16_t calculatedCrc16_back = server_global_functools_Crc::CalculateCrc16(BackDataFull.data(), BackDataFull.size());
    std::vector<uint8_t> calculatedCrc16_back_uint8 = server_global_functools_uintChange::uint16ToVector(calculatedCrc16_back);
    BackDataFull.insert(BackDataFull.end(),calculatedCrc16_back_uint8.begin(),calculatedCrc16_back_uint8.end());//放入crc16校验码
    BackDataFull.insert(BackDataFull.begin(),Recv_head.begin(),Recv_head.end());            // 放入头部
    BackDataFull.insert(BackDataFull.end(),Recv_end.begin(),Recv_end.end());                // 放入尾部
    send(client_socket, BackDataFull.data(), BackDataFull.size(), 0);                       // 返回消息
    std::cout << "返回‘" << BackDataMode << "’数据成功!" << std::endl;
}