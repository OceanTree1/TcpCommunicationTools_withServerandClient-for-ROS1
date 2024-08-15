#include "std_library.h"

class server_global_functools_print{
public:
    static void print_vector_uint8t(std::string name, std::vector<uint8_t> byte);   // 顺序打印vector容器中的uint8_t类型数据
    static void print_vector_int(std::string name, std::vector<int> intnum);        // 顺序打印vector容器中的int类型数据
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

class server_global_functools_return{
public:
    static std::vector<uint8_t> dataMergence_template(std::vector<uint8_t> ID_buffer, std::vector<uint8_t> Function_buffer, std::vector<uint8_t> databuffer, std::vector<uint8_t> DeviceID_buffer);
};