#include "IPCServerTools_v1/std_library.h"
#include "IPCServerTools_v1/global_func.h"

std::vector<uint8_t> server_global_functools_return::dataMergence_template(std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, std::vector<uint8_t> databuffer, std::vector<uint8_t> DeviceID_buffer){
    std::vector<uint8_t> Controller_send_template = {};       //初始化成功消息
    Controller_send_template.insert(Controller_send_template.end(),ID_buffer.begin(),ID_buffer.end());              // 放入序号
    Controller_send_template.insert(Controller_send_template.end(),Function_buffer.begin(),Function_buffer.end());  // 放入功能码
    Controller_send_template.insert(Controller_send_template.end(),databuffer.begin(),databuffer.end());            // 放入成功码
    Controller_send_template.insert(Controller_send_template.end(),DeviceID_buffer.begin(),DeviceID_buffer.end());            // 放入成功码
    Controller_send_template.insert(Controller_send_template.begin(),server_share_data::Recv_head.begin(),server_share_data::Recv_head.end());            // 放入头部
    Controller_send_template.insert(Controller_send_template.end(),server_share_data::Recv_end.begin(),server_share_data::Recv_end.end());                // 放入尾部
    return Controller_send_template;
}

//返回成功码拼接
bool server_global_functools_return::ReturnSuccessful(int client_socket, std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, uint16_t ErrorCode, std::vector<uint8_t> DeviceID_buffer){
    std::vector<uint8_t> Successful_code = server_global_functools_uintChange::uint16ToVector(ErrorCode);
    std::vector<uint8_t> Controller_send_Successful = dataMergence_template(ID_buffer, Function_buffer, Successful_code, DeviceID_buffer);       //初始化成功消息
    send(client_socket, Controller_send_Successful.data(), Controller_send_Successful.size(), 0);                       // 发送成功信息
    std::cout << "已顺利返回“成功”Message给上位机" << std::endl;
    return true;
}

//返回上位机失败拼接
bool server_global_functools_return::ReturnFalse(int client_socket, std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, uint16_t ErrorCode, std::vector<uint8_t> DeviceID_buffer){
    std::vector<uint8_t> False_code = server_global_functools_uintChange::uint16ToVector(ErrorCode);
    std::vector<uint8_t> Controller_send_False = dataMergence_template(ID_buffer, Function_buffer, False_code, DeviceID_buffer);// 初始化失败消息
    send(client_socket, Controller_send_False.data(), Controller_send_False.size(), 0);                         // 发送失败消息
    std::cout << "已顺利返回“失败”Message给上位机" << std::endl;
    return true;
}

bool server_global_functools_return::ReturnData(int client_socket, std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, std::vector<uint8_t> BackData, std::string BackDataMode, std::vector<uint8_t> DeviceID_buffer){
    std::vector<uint8_t> BackDataFull = dataMergence_template(ID_buffer, Function_buffer, BackData, DeviceID_buffer);       //初始化返回消息
    send(client_socket, BackDataFull.data(), BackDataFull.size(), 0);                       // 返回消息
    std::cout << "返回‘" << BackDataMode << "’数据成功!" << std::endl;
    return true;
}