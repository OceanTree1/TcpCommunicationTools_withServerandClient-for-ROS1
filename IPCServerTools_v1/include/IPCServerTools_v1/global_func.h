#include "std_library.h"

class server_global_functools_Crc {
public:
    uint16_t CalculateCrc16(const uint8_t* data, size_t length, uint16_t initialValue = 0xFFFF, uint16_t polynomial = 0xA001);
    bool CutDataforcrc16(const std::vector<uint8_t>& recBytes);
};


class server_global_functools_uintChange {
public:
    std::uint32_t vectorToUint32(const std::vector<std::uint8_t>& bytes);   // 将std::vector<uint8_t>转换为uint32_t
    std::vector<std::uint8_t> uint16ToVector(std::uint16_t value);          // 将uint16_t转换为std::vector<uint8_t>
    std::vector<bool> bytesToBinaryArray(const std::vector<std::uint8_t>& bytes);    // 将一组字节转换为二进制数组
    std::vector<std::uint8_t> uint32ToVector(std::uint32_t value);           // 将uint32_t转换为std::vector<uint8_t>
    std::uint16_t vectorToUint16(const std::vector<std::uint8_t>& bytes);    // 将std::vector<uint8_t>转换为uint16_t
    bool isBigEndian();                                                      // 检查系统是否为大端
    std::vector<std::uint8_t> doubleToVector(double value, bool toBigEndian = false);// 将double转换为std::vector<uint8_t>
};