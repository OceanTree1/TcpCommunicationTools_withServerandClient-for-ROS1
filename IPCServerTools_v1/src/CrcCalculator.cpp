#include <iostream>
#include <vector>
#include <cstdint>
#include "IPCServerTools_v1/global_func.h"

class CrcCalculator : server_global_func {
public:
    // crc16校验
    static uint16_t CalculateCrc16(const uint8_t* data, size_t length, uint16_t initialValue = 0xFFFF, uint16_t polynomial = 0xA001) {
        uint16_t crc = initialValue;
        for (size_t i = 0; i < length; ++i) {
            crc ^= data[i] << 8;
            for (int j = 0; j < 8; ++j) {
                if (crc & 0x8000)
                    crc = (crc << 1) ^ polynomial;
                else
                    crc = crc << 1;
            }
        }
        return crc;
    }

    bool CutDataforcrc16(const std::vector<uint8_t>& recBytes) { //CutData功能：报文切割
        size_t DataFullLength = recBytes.size();
        size_t crcDatasIndex = 4;
        size_t crcDatasLength = DataFullLength - 8 - 2;
        
        // crc16算法调用
        CrcCalculator crcCalculator;
        std::vector<uint8_t> Datas_point(recBytes.begin() + crcDatasIndex, recBytes.begin() + crcDatasIndex + crcDatasLength);
        // 调用 CRC16 计算器
        uint16_t calculatedCrc16 = CalculateCrc16(Datas_point.data(), Datas_point.size());
        // 提取原始数组中的 CRC16 数据
        size_t crc16StartIndex = DataFullLength - 6; // 原始 CRC16 数据在数组中的起始索引
        uint16_t originalCrc16 = (recBytes[crc16StartIndex] << 8) | recBytes[crc16StartIndex + 1];
        // 比较计算得到的 CRC16 与原始数组中的 CRC16
        if (calculatedCrc16 == originalCrc16) {
            // 在这里使用 crc16Result，例如输出到控制台
            std::cout << "CRC16 Result: " << std::hex << calculatedCrc16 << std::dec << std::endl;
            return true;
        } else {
            std::cout << "CRC16 Check Failed!" << std::endl;
            return false;
        }
    }
};