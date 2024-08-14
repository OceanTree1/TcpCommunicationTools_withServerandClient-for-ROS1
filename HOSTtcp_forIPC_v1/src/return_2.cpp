#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include "CutRecData.cpp"

std::vector<uint8_t> CodeLength = {0x00, 0x02};

std::vector<uint8_t> ReturnData(std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, std::vector<uint8_t> BackData, std::vector<uint8_t> Recv_head, std::vector<uint8_t> Recv_end){
    std::vector<uint8_t> BackDataFull = {};       //初始化返回消息
    uint16_t BackData_uint16 = static_cast<uint8_t>(BackData.size());
    std::vector<uint8_t> BackDataLength = uint16ToVector(BackData_uint16);
    BackDataFull.insert(BackDataFull.end(),ID_buffer.begin(),ID_buffer.end());              // 放入自增序列
    BackDataFull.insert(BackDataFull.end(),Function_buffer.begin(),Function_buffer.end());  // 放入功能码
    BackDataFull.insert(BackDataFull.end(),BackDataLength.begin(),BackDataLength.end());    // 放入长度
    BackDataFull.insert(BackDataFull.end(),BackData.begin(),BackData.end());                // 放入数据
    uint16_t calculatedCrc16_back = CrcCalculator::Crc16(BackDataFull.data(), BackDataFull.size());
    std::vector<uint8_t> calculatedCrc16_back_uint8 = uint16ToVector(calculatedCrc16_back);
    BackDataFull.insert(BackDataFull.end(),calculatedCrc16_back_uint8.begin(),calculatedCrc16_back_uint8.end());//放入crc16校验码
    BackDataFull.insert(BackDataFull.begin(),Recv_head.begin(),Recv_head.end());            // 放入头部
    BackDataFull.insert(BackDataFull.end(),Recv_end.begin(),Recv_end.end());                // 放入尾部
    // send(client_socket, BackDataFull.data(), BackDataFull.size(), 0);                       // 返回消息
    // std::cout << "返回‘" << BackDataMode << "’数据成功!" << std::endl;
    return BackDataFull;
}