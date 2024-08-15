#include "HOSTtcp_forIPC_v1/std_library.h"
#include "HOSTtcp_forIPC_v1/global_func.h"
#include "HOSTtcp_forIPC_v1/global_data.h"

std::vector<uint8_t> server_global_functools_return::dataMergence_template(std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, std::vector<uint8_t> databuffer, std::vector<uint8_t> DeviceID_buffer){
    std::vector<uint8_t> Controller_send_template = {};       //初始化成功消息
    Controller_send_template.insert(Controller_send_template.end(),ID_buffer.begin(),ID_buffer.end());              // 放入序号
    Controller_send_template.insert(Controller_send_template.end(),Function_buffer.begin(),Function_buffer.end());  // 放入功能码
    Controller_send_template.insert(Controller_send_template.end(),databuffer.begin(),databuffer.end());            // 放入成功码
    Controller_send_template.insert(Controller_send_template.end(),DeviceID_buffer.begin(),DeviceID_buffer.end());            // 放入成功码
    Controller_send_template.insert(Controller_send_template.begin(),server_global_data::Head.begin(),server_global_data::Head.end());            // 放入头部
    Controller_send_template.insert(Controller_send_template.end(),server_global_data::End.begin(),server_global_data::End.end());                // 放入尾部
    return Controller_send_template;
}