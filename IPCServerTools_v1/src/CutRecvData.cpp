#include "IPCServerTools_v1/std_library.h"
#include "IPCServerTools_v1/global_func.h"

// ProcessResult功能：将所有完整帧报文处理在一个容器里面存放
void CutData::ProcessResult(int startIndex, int endIndex, std::vector<std::vector<uint8_t>>& buffer){
    int length = endIndex - startIndex + server_share_data::Recv_end.size();
    std::vector<uint8_t> data(recBuffer.begin() + startIndex, recBuffer.begin() + startIndex + length);
    buffer.emplace_back(data);
}

//Process功能：截取完整帧报文
std::vector<std::vector<uint8_t>> CutData::Process(const std::vector<uint8_t>& recBytes) {
    std::vector<std::vector<uint8_t>> recv_uint8_buffer;

    recBuffer.insert(recBuffer.end(), recBytes.begin(), recBytes.end());

    int currentIndex = 0;

    while (currentIndex <= recBuffer.size()) {
        int startIndex = FindSegment(recBuffer, server_share_data::Recv_head, currentIndex);
        if (startIndex < 0) {
            RemoveProcessedData(currentIndex);
            break;
        }

        int endIndex = FindSegment(recBuffer, server_share_data::Recv_end, startIndex + server_share_data::Recv_head.size());
        if (endIndex < 0) {
            RemoveProcessedData(startIndex);
            break;
        }

        ProcessResult(startIndex, endIndex, recv_uint8_buffer);
        currentIndex = endIndex + server_share_data::Recv_end.size();
    }

    return recv_uint8_buffer;
}

// 偏暴力算法，去除KMP中的LPS（next数组）查找，因为报文头尽量都要是不同的，如有相同需要，可以加入KMP算法
int CutData::FindSegment(const std::vector<uint8_t>& recBuffer, const std::vector<uint8_t>& sequence, int startIndex){
        int i = startIndex;
        int j = 0;

        while (i < recBuffer.size()) {
            if (sequence[j] == recBuffer[i]) {
                j++;
                i++;
            }
            if (j == sequence.size()) {
                return i - j;
            } else if (i < recBuffer.size() && sequence[j] != recBuffer[i]) {
                if (j != 0)
                    j = 0;
                else
                    i++;
            }
        }
        return -1;
}

void CutData::RemoveProcessedData(int startIndex) {
    if (startIndex > 0) {
        recBuffer.erase(recBuffer.begin(), recBuffer.begin() + startIndex);
    }
}   