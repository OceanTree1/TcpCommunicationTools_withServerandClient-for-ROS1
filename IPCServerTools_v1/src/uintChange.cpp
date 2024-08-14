#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring> // For std::memcpy
#include <algorithm> // For std::reverse
#include "IPCServerTools_v1/global_func.h"


// 将std::vector<uint8_t>转换为uint32_t
std::uint32_t server_global_functools_uintChange::vectorToUint32(const std::vector<std::uint8_t>& bytes) {
    if (bytes.size() != sizeof(std::uint32_t)) {
        throw std::runtime_error("Vector size does not match uint32_t size");
    }

    std::uint32_t result = 0;
    for (size_t i = 0; i < sizeof(std::uint32_t); ++i) {
        result = (result << 8) | static_cast<std::uint32_t>(bytes[i]);
    }

    return result;
}


// 将uint16_t转换为std::vector<uint8_t>
std::vector<std::uint8_t> server_global_functools_uintChange::uint16ToVector(std::uint16_t value) {
    std::vector<std::uint8_t> result(sizeof(std::uint16_t));

    for (size_t i = 0; i < sizeof(std::uint16_t); ++i) {
        result[i] = static_cast<std::uint8_t>(value >> (8 * (sizeof(std::uint16_t) - 1 - i)));
    }

    return result;
}

// 将一组字节转换为二进制数组
std::vector<bool> server_global_functools_uintChange::bytesToBinaryArray(const std::vector<std::uint8_t>& bytes) {
    std::vector<bool> binaryArray;

    for (const auto& byte : bytes) {
        for (int i = 7; i >= 0; --i) {
            binaryArray.push_back((byte >> i) & 1);
        }
    }

    return binaryArray;
}

// 将uint32_t转换为std::vector<uint8_t>
std::vector<std::uint8_t> server_global_functools_uintChange::uint32ToVector(std::uint32_t value) {
    std::vector<std::uint8_t> result(sizeof(std::uint32_t));

    for (size_t i = 0; i < sizeof(std::uint32_t); ++i) {
        result[sizeof(std::uint32_t) - 1 - i] = static_cast<std::uint8_t>(value >> (i * 8));
    }

    return result;
}

// 将std::vector<uint8_t>转换为uint16_t
std::uint16_t server_global_functools_uintChange::vectorToUint16(const std::vector<std::uint8_t>& bytes) {
    if (bytes.size() != sizeof(std::uint16_t)) {
        throw std::runtime_error("Vector size does not match uint16_t size");
    }

    std::uint16_t result = 0;
    for (size_t i = 0; i < sizeof(std::uint16_t); ++i) {
        result = (result << 8) | static_cast<std::uint16_t>(bytes[i]);
    }

    return result;
}

// 将double转换为std::vector<uint8_t>
// 检查系统是否为大端
bool server_global_functools_uintChange::isBigEndian() {
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}
std::vector<std::uint8_t> server_global_functools_uintChange::doubleToVector(double value, bool toBigEndian = false) {
    std::vector<std::uint8_t> bytes(sizeof(double));
    std::memcpy(bytes.data(), &value, sizeof(double));
    // 如果需要，根据当前系统的字节序和目标字节序调整字节顺序
    if (toBigEndian != isBigEndian()) {
        std::reverse(bytes.begin(), bytes.end());
    }
    return bytes;
}
