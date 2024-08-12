#include "std_library.h"

class server_global_functools_Crc {
public:
    static uint16_t CalculateCrc16(const uint8_t* data, size_t length, uint16_t initialValue = 0xFFFF, uint16_t polynomial = 0xA001);
    static bool CutDataforcrc16(const std::vector<uint8_t>& recBytes);
};


class server_global_functools_uintChange {
public:
    static std::uint32_t vectorToUint32(const std::vector<std::uint8_t>& bytes);   // 将std::vector<uint8_t>转换为uint32_t
    static std::vector<std::uint8_t> uint16ToVector(std::uint16_t value);          // 将uint16_t转换为std::vector<uint8_t>
    static std::vector<bool> bytesToBinaryArray(const std::vector<std::uint8_t>& bytes);    // 将一组字节转换为二进制数组
    static std::vector<std::uint8_t> uint32ToVector(std::uint32_t value);           // 将uint32_t转换为std::vector<uint8_t>
    static std::uint16_t vectorToUint16(const std::vector<std::uint8_t>& bytes);    // 将std::vector<uint8_t>转换为uint16_t
    static bool isBigEndian();                                                      // 检查系统是否为大端
    static std::vector<std::uint8_t> doubleToVector(double value, bool toBigEndian = false);// 将double转换为std::vector<uint8_t>
};