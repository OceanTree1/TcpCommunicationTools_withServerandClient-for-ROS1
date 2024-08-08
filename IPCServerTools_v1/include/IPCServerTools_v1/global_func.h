#include "std_library.h"

class server_global_func {
public:
    static uint16_t CalculateCrc16(const uint8_t* data, size_t length, uint16_t initialValue = 0xFFFF, uint16_t polynomial = 0xA001);
    bool CutDataforcrc16(const std::vector<uint8_t>& recBytes);
};
