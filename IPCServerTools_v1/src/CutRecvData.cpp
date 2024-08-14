#include "IPCServerTools_v1/std_library.h"
#include "IPCServerTools_v1/global_func.h"

extern const std::vector<uint8_t> server_share_data::Recv_head;
extern const std::vector<uint8_t> server_share_data::Recv_end;


std::vector<std::vector<uint8_t>> CutData::Process(const std::vector<uint8_t>& recBytes) { //Process功能：截取完整帧报文
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

        ProcessDataSegment(startIndex, endIndex, recv_uint8_buffer);
        currentIndex = endIndex + server_share_data::Recv_end.size();
    }

    return recv_uint8_buffer;
}

int CutData::FindSegment(const std::vector<uint8_t>& recBuffer, const std::vector<uint8_t>& sequence, int startIndex = 0){
    // std::vector<int> lps = ComputeLPSArray(sequence);
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
                    // j = lps[j - 1];
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