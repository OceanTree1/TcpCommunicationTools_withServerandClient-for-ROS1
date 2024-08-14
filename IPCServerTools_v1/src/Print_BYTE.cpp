#include "IPCServerTools_v1/std_library.h"
#include "IPCServerTools_v1/global_func.h"

// 顺序打印vector容器中的uint8_t类型数据
void server_global_functools_print::print_vector_uint8t(std::string name, std::vector<uint8_t> byte){
    std::cout << name << ": " ;
    for (int i = 0; i < byte.size(); ++i) {   
        std::cout << "0x" << std::hex << static_cast<int>(byte[i]) << " ";
    }
    std::cout << std::dec << std::endl; // 打印结束
}

// 顺序打印vector容器中的int类型数据
void server_global_functools_print::print_vector_int(std::string name, std::vector<int> intnum){
    std::cout << name << ": " ;
    for (int i = 0; i < intnum.size(); ++i) {   
        std::cout <<  static_cast<int>(intnum[i]) << " ";
    }
    std::cout << std::endl;
}