#include "std_library.h"

class server_global_data {
public:
    // 全局报文头部与尾部
    static const std::vector<uint8_t> Head;      // 头部
    static const std::vector<uint8_t> End;       // 尾部

    // 全局原子标志位
    static bool running_tcp;

    // 全局反馈数据
    static std::vector<uint8_t> Information_Feedback;  // 反馈数据举例

    // 设备识别码
    static const std::vector<uint8_t> deviceID_input;

    static uint32_t listNum;
};