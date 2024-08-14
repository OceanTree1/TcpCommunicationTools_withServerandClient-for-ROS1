#include "IPCServerTools_v1/std_library.h"
#include "IPCServerTools_v1/global_func.h"

bool server_global_functools_deviceID::deviceID_judging(const std::uint32_t bytes){
    if (bytes == 0x00000001) {
        return true;
    } else {
        return false;
    }
}