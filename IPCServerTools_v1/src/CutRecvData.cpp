#include "IPCServerTools_v1/std_library.h"
#include "IPCServerTools_v1/global_func.h"

extern const std::vector<uint8_t> server_share_data::Recv_head;
extern const std::vector<uint8_t> server_share_data::Recv_end;


std::vector<uint8_t> CutData::Process(const std::vector<uint8_t>& recBytes) { //Process功能：截取完整帧报文
    std::vector<uint8_t> recBuffer;
    recBuffer.insert(recBuffer.end(), recBytes.begin(), recBytes.end());
    int currentIndex = 0;

    while (currentIndex <= recBuffer.size()) {
        int startIndex = FindSequence(recBuffer, server_share_data::Recv_head, currentIndex);
        if (startIndex < 0) {
            RemoveProcessedData(currentIndex);
            break;
        }

        int endIndex = FindSequence(recBuffer, server_share_data::Recv_end, startIndex + server_share_data::Recv_head.size());
        if (endIndex < 0) {
            RemoveProcessedData(startIndex);
            break;
        }

        ProcessDataSegment(startIndex, endIndex, pList);
        currentIndex = endIndex + recvend.size();
    }

    return pList;
}