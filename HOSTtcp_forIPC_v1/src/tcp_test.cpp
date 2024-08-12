#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include "return_2.cpp"
#include "Print_BYTE.cpp"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

uint32_t listNum = 0;
std::vector<uint8_t> Head{0xAB, 0xCD, 0xEF};
std::vector<uint8_t> End{0x12, 0x34, 0x56};
std::vector<uint8_t> Function_buffer;
bool running_tcp = true;

void send_message(int socket_fd, std::vector<uint8_t> Data) {
    if (send(socket_fd, Data.data(), Data.size(), 0) == -1) {
        std::cerr << "Failed to send message\n";
    } else {
        listNum++;
        
        // std::cout << "Message sent: " << BackDataFull.data() << "\n";
        print_vector_uint8t("Message sent", Data);
    }
}

void heartjump(int socket_fd){
    // ros::Rate r(1);
    while (running_tcp){
        std::vector<uint8_t> Function_buffer{0x58, 0x54, 0x69, 0x6E};
        std::vector<uint8_t> Data_input{};
        std::vector<uint8_t> Full_Data{};
        // Full_Data.insert(Full_Data.begin(), Head.begin(), Head.end());
        std::vector<uint8_t> ID_buffer = uint32ToVector(listNum);
        Full_Data = ReturnData(ID_buffer, Function_buffer, Data_input, Head, End);
        send_message(socket_fd, Full_Data);
        sleep(2);
    }
}

int main() {
    // Create socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        std::cerr << "Failed to create socket\n";
        return 0;
    }

    // Define server address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Connect to server
    if (connect(socket_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server\n";
        close(socket_fd);
        return 0;
    }

    std::cout << "Connected to server\n";

    std::thread(heartjump, socket_fd).detach();
    // Main loop
    while (true) {
        std::cout << "Select an option:\n";
        std::cout << "1. 发送行进点\n";
        std::cout << "2. 启动与关闭行进模式\n";
        std::cout << "3. 发送自动点、区域\n";
        std::cout << "4. 启动/关闭自动模式\n";

        int choice;
        std::cin >> choice;
        
        if (choice == 1) {  // 发送行进点
            std::vector<uint8_t> Function_buffer{0x58, 0x4A, 0x51, 0x59};
            std::vector<uint8_t> Data_input{0x00, 0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x05};
            std::vector<uint8_t> Full_Data{};
            // Full_Data.insert(Full_Data.begin(), Head.begin(), Head.end());
            std::vector<uint8_t> ID_buffer = uint32ToVector(listNum);
            Full_Data = ReturnData(ID_buffer, Function_buffer, Data_input, Head, End);
            send_message(socket_fd, Full_Data);
        } else if (choice == 2) {   // 启动与关闭行进模式
            std::vector<uint8_t> Function_buffer{0x4C, 0x4A, 0x58, 0x4A};
            std::vector<uint8_t> Data_input{0x00};
            std::cout << "(1)启动,(2)停止" << std::endl;
            // std::string BackDataMode = "发送行进点";
            std::vector<uint8_t> Full_Data{};
            int choice_zidong;
            std::cin >> choice_zidong;
            if (choice_zidong == 1) {
                Data_input[0] = 1;
            } else {
                Data_input[0] = 0;
            }
            std::vector<uint8_t> ID_buffer = uint32ToVector(listNum);
            Full_Data = ReturnData(ID_buffer, Function_buffer, Data_input, Head, End);
            send_message(socket_fd, Full_Data);
        } else if (choice == 3) {   // 发送工作区域
            std::vector<uint8_t> Function_buffer{0x47, 0x5A, 0x51, 0x59};
            std::vector<uint8_t> Data_input{0x00, 0x0E, 0x00, 0x05, 0x00, 0x06, 0x00, 0x14, 0x00, 0x09, 0x00, 0x06, 0x00, 0x01, 0x00, 0x14, 0x00, 0x03, 0x00, 0x06, 0x04};
            std::vector<uint8_t> Full_Data{};
            std::vector<uint8_t> ID_buffer = uint32ToVector(listNum);
            Full_Data = ReturnData(ID_buffer, Function_buffer, Data_input, Head, End);
            send_message(socket_fd, Full_Data);
        } else if (choice == 4) {   // 启动与关闭自动模式
            std::vector<uint8_t> Function_buffer{0x5A, 0x44, 0x5A, 0x59};
            std::vector<uint8_t> Data_input{0x00};
            std::cout << "(1)启动,(2)停止" << std::endl;
            std::vector<uint8_t> Full_Data{};
            int choice_zidong;
            std::cin >> choice_zidong;
            if (choice_zidong == 1) {
                Data_input[0] = 1;
            } else {
                Data_input[0] = 0;
            }
            std::vector<uint8_t> ID_buffer = uint32ToVector(listNum);
            Full_Data = ReturnData(ID_buffer, Function_buffer, Data_input, Head, End);
            send_message(socket_fd, Full_Data);
        } else if (choice == 5) {   
            std::vector<uint8_t> Function_buffer{0x58, 0x4A, 0x51, 0x59};
            std::vector<uint8_t> Data_input{0x00, 0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02};
            std::vector<uint8_t> Full_Data{};
            // Full_Data.insert(Full_Data.begin(), Head.begin(), Head.end());
            std::vector<uint8_t> ID_buffer = uint32ToVector(listNum);
            Full_Data = ReturnData(ID_buffer, Function_buffer, Data_input, Head, End);
            send_message(socket_fd, Full_Data);
        } else if (choice == 6){
            running_tcp = false;
            break;
        } else {
            std::cout << "Invalid choice, please try again\n";
        }
    }
    
    // Close socket
    close(socket_fd);
    return 0;
}