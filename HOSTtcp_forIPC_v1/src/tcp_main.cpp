#include "HOSTtcp_forIPC_v1/std_library.h"
#include "HOSTtcp_forIPC_v1/global_func.h"
#include "HOSTtcp_forIPC_v1/global_data.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

const std::vector<uint8_t> server_global_data::Head{0xAB, 0xCD, 0xEF};
const std::vector<uint8_t> server_global_data::End{0x12, 0x34, 0x56};
const std::vector<uint8_t> server_global_data::deviceID_input{0x00, 0x00, 0x00, 0x01};
bool server_global_data::running_tcp = true;

class Tcp_client : server_global_data {
public:
    static void send_message(int socket_fd, std::vector<uint8_t> Data) {
        if (send(socket_fd, Data.data(), Data.size(), 0) == -1) {
            std::cerr << "Failed to send message\n";
        } else {
            listNum++;
            
            // std::cout << "Message sent: " << BackDataFull.data() << "\n";
            server_global_functools_print::print_vector_uint8t("Message sent", Data);
        }
    }

    static void heartjump(int socket_fd){
        // ros::Rate r(1);
        while (running_tcp){
            std::vector<uint8_t> Function_buffer{0x58, 0x54, 0x69, 0x6E};
            std::vector<uint8_t> Data_input{};
            std::vector<uint8_t> Full_Data{};
            // Full_Data.insert(Full_Data.begin(), Head.begin(), Head.end());
            std::vector<uint8_t> ID_buffer = server_global_functools_uintChange::uint32ToVector(listNum);
            Full_Data = server_global_functools_return::dataMergence_template(ID_buffer, Function_buffer, Data_input, deviceID_input);
            send_message(socket_fd, Full_Data);
            sleep(2);
        }
    }
    
    static std::vector<uint8_t> returnFulldata(std::vector<uint8_t> Function_buffer, std::vector<uint8_t> Data_input) {
        std::vector<uint8_t> Full_Data{};
        // Full_Data.insert(Full_Data.begin(), Head.begin(), Head.end());
        std::vector<uint8_t> ID_buffer = server_global_functools_uintChange::uint32ToVector(server_global_data::listNum);
        Full_Data = server_global_functools_return::dataMergence_template(ID_buffer, Function_buffer, Data_input, server_global_data::deviceID_input);
        return Full_Data;
    }
};

uint32_t server_global_data::listNum = 0;

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
    std::thread(Tcp_client::heartjump, socket_fd).detach();
    Tcp_client tcp_client;
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
            std::vector<uint8_t> Function_buffer{0x11, 0x11, 0x11, 0x12};
            std::vector<uint8_t> Data_input{0x00, 0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x05};
            std::vector<uint8_t> Full_Data = tcp_client.returnFulldata(Function_buffer, Data_input);
            tcp_client.send_message(socket_fd, Full_Data);

        } else if (choice == 2) {   // 启动与关闭行进模式
            std::vector<uint8_t> Function_buffer{0x11, 0x11, 0x11, 0x12};
            std::vector<uint8_t> Data_input{0x00};
            std::cout << "(1)启动,(2)停止" << std::endl;
            // std::string BackDataMode = "发送行进点";
            int choice_zidong;
            std::cin >> choice_zidong;
            if (choice_zidong == 1) {
                Data_input[0] = 1;
            } else {
                Data_input[0] = 0;
            }
            std::vector<uint8_t> Full_Data = tcp_client.returnFulldata(Function_buffer, Data_input);
            tcp_client.send_message(socket_fd, Full_Data);

        } else if (choice == 3) {   // 发送工作区域
            std::vector<uint8_t> Function_buffer{0x11, 0x11, 0x11, 0x13};
            std::vector<uint8_t> Data_input{0x00, 0x0E, 0x00, 0x05, 0x00, 0x06, 0x00, 0x14, 0x00, 0x09, 0x00, 0x06, 0x00, 0x01, 0x00, 0x14, 0x00, 0x03, 0x00, 0x06, 0x04};
            std::vector<uint8_t> Full_Data = tcp_client.returnFulldata(Function_buffer, Data_input);
            tcp_client.send_message(socket_fd, Full_Data);

        } else if (choice == 4) {   // 启动与关闭自动模式
            std::vector<uint8_t> Function_buffer{0x5A, 0x44, 0x5A, 0x59};
            std::vector<uint8_t> Data_input{0x00}; 
            std::cout << "(1)启动,(2)停止" << std::endl;
            int choice_zidong;
            std::cin >> choice_zidong;
            if (choice_zidong == 1) {
                Data_input[0] = 1;
            } else {
                Data_input[0] = 0;
            }
            std::vector<uint8_t> Full_Data = tcp_client.returnFulldata(Function_buffer, Data_input);
            tcp_client.send_message(socket_fd, Full_Data);
        } else if (choice == 5) {   
            std::vector<uint8_t> Function_buffer{0x11, 0x11, 0x11, 0x14};
            std::vector<uint8_t> Data_input{0x00, 0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02};
            std::vector<uint8_t> Full_Data = tcp_client.returnFulldata(Function_buffer, Data_input);
            tcp_client.send_message(socket_fd, Full_Data);
        } else if (choice == 6){
            server_global_data::running_tcp = false;
            break;
        } else {
            std::cout << "Invalid choice, please try again\n";
        }
    }
    
    // Close socket
    close(socket_fd);
    return 0;
}