#include "IPCServerTools_v1/std_library.h"
#include "return.cpp"
#include "Print_BYTE.cpp"
#include "IPCServerTools_v1/global_func.h"

#define MAX_BUFFER_SIZE 4096    // 最大size
#define MAX_CLIENTS 5           // 最大客户端连接数
#define PORT 12345              // 端口设置

class server_IPC : server_share_data {
public:
    // Tcp客户端连接超时时间
    static const int TIMEOUT_MS;

    // 线程、客户端与锁
    static std::mutex client_mutex;
    static std::condition_variable cv;
    static std::vector<int> clients;

    // 反馈数据定义
    static std::vector<uint8_t> Information_Feedback;  // 反馈数据举例

    //***************************************

    // 参数初始化函数
    static void finish_init(){
        /**********初始化内容*********/
        // 例如：某业务变量初始化为0

        /****************************/
    }

    // 信号处理函数
    static void signal_handler(int signal) {
        if (signal == SIGINT) {
            std::cout << "SIGINT received, stopping thread..." << std::endl;
            running_Main = false;       // 主函数的循环管理
            running_Modbus = false;     // modbus线程的循环管理
        }
    }

    // Socket超时函数判断
    static void configure_timeout(int socket) {
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_MS / 1000;  // 秒
        timeout.tv_usec = (TIMEOUT_MS % 1000) * 1000;  // 微秒

        // 设置接收超时
        if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
            std::cerr << "Error setting socket receive timeout." << std::endl;
        }
    }

    // 处理连接主函数
    static void handle_client(int client_socket) {
        // 配置线程与服务端的部分初始参数
        std::unique_lock<std::mutex> lock(client_mutex);
        server_IPC::configure_timeout(client_socket);   // 配置每个客户端的超时
        clients.push_back(client_socket);   // 将新的客户端添加到连接列表
        cv.notify_all();    // 唤醒等待的线程
        lock.unlock();  // 线程解锁
        
        // **************ros定义内容*****************
        ros::NodeHandle node_handle;
        // ros::Publisher xx = node_handle.advertise<std_msgs::Float32MultiArray>("/xx" ,10);  // 判断是哪个模式是否启动
        // *****************************************

        int bytes_received; // 接收字节数

        ClipRecDataProcessor cliprecdataprocessor;  //裁帧类
        ConcurrentDictionary<uint32_t,std::vector<uint8_t>> myDictionary;   //字典类
        
        unsigned char recv_Msg_buffer[MAX_BUFFER_SIZE];
        uint32_t oldrecvID = {};                    //之前一条报文的ID

        while (true) {
            // 清空缓存区
            memset(recv_Msg_buffer, 0, sizeof(recv_Msg_buffer));  //消息空间清空
            std::vector<uint8_t> Process_Msg_buffer;   //处理消息缓存区
            std::vector<uint8_t> Processready_Msg_buffer;   //处理完成缓存区
            std::vector<ProcessedData> recvofdataList_Msg_buffer;   //处理完成后数据部分缓存区

            // 接收客户端数据并判断是否在线
            bytes_received = recv(client_socket, recv_Msg_buffer, sizeof(recv_Msg_buffer), 0);  // 接收客户端发送的数据
            if (bytes_received > 0) {
                // 正常接收数据，可以处理并回送
            } else if (bytes_received == 0) {
                // 超时或客户端断开连接
                std::cout << "Client timed out: " << client_socket << std::endl;
                std::cout << "由于客户端: " << client_socket << " 超时连接所以准备清零" << std::endl;
                finish_init();
                std::cout << "清零结束" << std::endl;
                break;
            } else if (bytes_received < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                std::cout << "Client disconnected: " << client_socket << std::endl;
                std::cout << "由于客户端: " << client_socket << " 断开所以准备清零" << std::endl;
                finish_init();
                std::cout << "清零结束" << std::endl;
                break;
            } else {
                // 其他错误
                std::cerr << "Receive error: " << strerror(errno) << std::endl;
                std::cout << "由于客户端: " << client_socket << " 断开所以准备清零" << std::endl;
                finish_init();
                std::cout << "清零结束" << std::endl;
                break;
            }


            // 打印与存储收到的数据
            std::cout << "Received: " ;
            for (int i = 0; i < bytes_received; ++i) {
                Process_Msg_buffer.push_back(static_cast<uint8_t>(recv_Msg_buffer[i])); // 把收到的数据依次推入Process_Msg_buffer中
                std::cout << "0x" << std::hex << static_cast<int>(recv_Msg_buffer[i]) << " ";   // 把收到的数据依次打印
            }
            std::cout << std::dec << std::endl;

            // 丢入Process函数进行处理，先截取到一帧完整的报文
            recvofdataList_Msg_buffer = cliprecdataprocessor.Process(Process_Msg_buffer);   //截取消息

            // For循环处理单帧数据
            for (const auto& processedData : recvofdataList_Msg_buffer) {   //遍历容器内所有数据，有几帧报文重复几次（切割单帧）
                // 访问 ProcessedData 中的成员
                std::vector<uint8_t> full_right_recv = {};  // 初始化单帧报文空间
                int intValue = processedData.intValue;
                std::vector<uint8_t> uint8Vector = processedData.uint8Vector;
                full_right_recv.insert(full_right_recv.end(), processedData.uint8Vector.begin(), processedData.uint8Vector.end());  // 拼接完整一帧报文
                
                // 截取自增码
                std::vector<uint8_t> ID_buffer(full_right_recv.begin() + 4, full_right_recv.begin() + 4 + 4);   // 取出id
                uint32_t IDuint32 = server_global_functools_uintChange::vectorToUint32(ID_buffer);  // 转换成uint32_t
                
                // 截取功能码
                std::vector<uint8_t> Function_buffer(full_right_recv.begin() + 8, full_right_recv.begin() + 8 + 4);// 取出功能码
                uint32_t Function_buffer32 = server_global_functools_uintChange::vectorToUint32(Function_buffer);// 转换成uint32_t
                std::cout << "FunctionBuffer: " ;   // 先打印ip地址，分辨从哪个客户端传来的
                for (int i = 0; i < Function_buffer.size(); ++i) {   // char转uint8_t
                    std::cout << "0x" << std::hex << static_cast<int>(Function_buffer[i]) << " ";
                }
                std::cout << std::dec << std::endl; // 打印结束

                // 初始化错误码
                uint16_t ErrorCode = 0x0000;    // 错误代码（包含正确代码，返回用）

                // 判断报文是否正确(crc16校验)
                if (cliprecdataprocessor.CutDataforcrc16(full_right_recv)) { 
                    // Crc16校验正确之后开始处理业务
                    
                    // 先与前一帧的ID对比，判断是否是一样的，一样的不处理
                    // 判断是否有这个Key以及Key下的Value是否在字典中已经存在
                    if(myDictionary.ContainsKey(IDuint32) && myDictionary.Compare(IDuint32,full_right_recv)){
                        std::cout << "收到重复帧，现在跳过" << std::endl;
                        continue;   // 不做反馈处理
                    } else {
                        myDictionary.Insert(IDuint32,full_right_recv);  // 插入字典
                    }
                    
                    //判断Data数据长度是否对应
                    int Datas_Length = full_right_recv.size() - 20; // 自己读取的data长度
                    std::vector<uint8_t> DatasSizespace_uint8(full_right_recv.begin() + 12, full_right_recv.begin() + 12 + 2);
                    int DataSizespace = static_cast<int>(server_global_functools_uintChange::vectorToUint16(DatasSizespace_uint8));
                    std::cout << "当前的DataSizespace为：" << DataSizespace << std::endl;
                    // 当数据有，并且源数据也不为0时
                    if ((full_right_recv[12] != 0 || full_right_recv[13] != 0) && Datas_Length != 0){
                        if(DataSizespace != Datas_Length){
                            std::cout << "自己算取的数据长度为: " << Datas_Length << ",跟源数据中的长度("<< DataSizespace <<")不对等" << std::endl;
                            continue;
                        }
                    // 当数据没有，并且源数据也为0时
                    } else if ((full_right_recv[12] == 0 && full_right_recv[13] == 0) && Datas_Length == 0) {
                        std::cout << "数据长度为0,和源数据长度(" << DataSizespace << ")对比通过" << std::endl;
                        // 请求反馈值的功能码case需要进行剔除
                        if (Function_buffer32 != 0x5854696E && Function_buffer32 != 0x585A696E && Function_buffer32 != 0x5A59696E && Function_buffer32 != 0x4459696E && Function_buffer32 != 0x4459706E) {
                            std::cout << "Error:控制命令数据不能为零" << std::endl;
                            ErrorCode = 0x0002; // 2为数据错误
                        }
                    // 其他情况
                    } else {
                        std::cout << "数据不对" << std::endl;
                        ErrorCode = 0x0002; // 2为数据错误
                    }
                    if (ErrorCode != 0x0000) {
                        //返回失败码
                        if (ReturnFalse(client_socket, ID_buffer, Function_buffer, ErrorCode, Recv_head, Recv_end)) {
                            std::cout << "反馈'False'成功" << std::endl;
                        }
                        continue;
                    }

                    //功能分支
                    try{
                        // 截取数据
                        std::vector<uint8_t> Datas_Send_buffer(full_right_recv.begin() + 14, full_right_recv.begin() + 14 + full_right_recv.size() - 19);   // 取出收来数据data
                        // 判断功能码
                        switch (Function_buffer32)
                        {
                        case 0x11111111:    // 心跳功能
                        {
                            ErrorCode = 0x0001;
                            std::cout << "心跳正常" << std::endl;
                            break;
                        }

                        //*********************** 数据写入Case *********************

                        case 0x11111112:    // 工作区域数据获取
                        {
                            // ****输入业务逻辑****
                            ErrorCode = 0x0001;
                            // *******************
                            break;
                        }

                        //*********************** 数据返回Case *********************
                        case 0x11111113:    // 行走状态信息返回
                        {
                            std::string BackDataMode = "行走状态信息";
                            // 读取数组返回数据并发送
                            if (ReturnData(client_socket, ID_buffer, Function_buffer, Information_Feedback, BackDataMode, Recv_head, Recv_end)) {
                                continue;
                            } else {
                                ErrorCode = 0x0003; // 3为数据返回Case未返回
                            }
                        }
                        
                        default:    // 出现未定义功能码
                        {
                            //返回失败码
                            ErrorCode = 0x0004; //  4为功能码错误（未定义功能）
                            break;
                        }
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Exception: " << e.what() << std::endl;
                        std::cout << "出现异常,功能跳过" << std::endl;
                        ErrorCode = 0x0010; // 功能内抛出异常，需要检查Case
                    }
                } else {
                    //返回失败码
                    ErrorCode = 0x0005; // 5为Crc16校验失败代码
                } 

                // 根据ErrorCode的值返回给上位机，同时把数据发送给车子
                if (ErrorCode == 0x0001 && Function_buffer32 != 0x5854696E) {
                    //返回成功码
                    if (ReturnSuccessful(client_socket, ID_buffer, Function_buffer, ErrorCode, Recv_head, Recv_end)) {
                        std::cout << "反馈'Success'成功" << std::endl;
                    }
                } else if (ErrorCode == 0x0001 && Function_buffer32 == 0x5854696E) {
                    //返回成功码
                    if (ReturnSuccessful(client_socket, ID_buffer, Function_buffer, ErrorCode, Recv_head, Recv_end)) {
                        std::cout << "心跳当前正常" << std::endl;
                    }
                } else if (ErrorCode == 0x0005) {
                    printf("Crc错误\n");
                    if (ReturnFalse(client_socket, ID_buffer, Function_buffer, ErrorCode, Recv_head, Recv_end)) {
                        std::cout << "反馈'False'成功" << std::endl;
                    }
                } else {
                    // 其余情况
                    printf("ErrorCode不为1,请及时处理!!");
                    // 返回失败码
                    if (ReturnFalse(client_socket, ID_buffer, Function_buffer, ErrorCode, Recv_head, Recv_end)) {
                        std::cout << "反馈'False'成功" << std::endl;
                    }
                }
            }
        }
        // 设置标志位为false，通知线程停止
        running_Modbus = false;

        // 等待线程停止（确保足够时间让线程退出）
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Main thread is exiting..." << std::endl;

        // 关闭客户端连接
        close(client_socket);

        // 在连接列表中移除客户端
        lock.lock();
        clients.erase(std::remove_if(clients.begin(), clients.end(),
                                    [client_socket](int socket) { return socket == client_socket; }),
                    clients.end());
    }


    // 设置套接字为非阻塞模式
    static void set_non_blocking(int sockfd) {
        int flags = fcntl(sockfd, F_GETFL, 0);
        if (flags == -1) {
            perror("fcntl(F_GETFL)");
            exit(EXIT_FAILURE);
        }

        flags |= O_NONBLOCK;
        if (fcntl(sockfd, F_SETFL, flags) == -1) {
            perror("fcntl(F_SETFL)");
            exit(EXIT_FAILURE);
        }
    }
};

// 定义变量与常量区域

// 原子位
std::atomic<bool> server_share_data::running_Main(true);   // 主线程持续运行变量（默认为true）
std::atomic<bool> server_share_data::running_Modbus(true); // modbus线程持续运行变量（默认为true）

// Tcp连接超时时间
const int server_IPC::TIMEOUT_MS = 30000;

std::mutex server_IPC::client_mutex;
std::condition_variable server_IPC::cv;
std::vector<int> server_IPC::clients;

const std::vector<uint8_t> server_share_data::Recv_head = {0xAB, 0xCD, 0xEF};      // 头部
const std::vector<uint8_t> server_share_data::Recv_end = {0x12, 0x34, 0x56};       // 尾部

std::vector<uint8_t> server_IPC::Information_Feedback{0x00, 0x00, 0x00, 0x01};  // 反馈数据举例

int main(int argc, char *argv[]) {
    // ROS部分初始化处理
    setlocale(LC_ALL,"zh_CN.utf8"); //中文乱码处理
    ros::init(argc,argv,"TcpServer_forROS"); //初始化节点，节点名为TcpServer_forROS

    // 对类进行实例化
    // server_IPC server_ipc;

    // 设置SIGINT信号处理器，用来关闭程序
    std::signal(SIGINT, server_IPC::signal_handler);    // 设置SIGINT信号处理器

    // 服务端的Socket与监听处理
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    // 服务端的参数初始化
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // 尝试绑定地址
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error binding to address." << std::endl;
        close(server_socket);
        return -1;
    }

    // 尝试监听socket
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Error listening for connections." << std::endl;
        close(server_socket);
        return -1;
    }

    // 若前面都正常，则证明TCPServer的ip和端口当前都为正常，准备开始监听（客户端可以根据打印的端口来送入数据）
    std::cout << "Server listening on port " << PORT << "..." << std::endl;

    server_IPC::set_non_blocking(server_socket);// 设置套接字为非阻塞模式

    // 启动与modbus服务器的连接（PS，如果需要的话可以加入modbus服务器的连接，此modbus若未连接会一直重连，如果不需要请别开启）
    // std::thread(modbus_client_func).detach();   // 开启连接modbus服务器函数线程

    // 尝试开始监听客户端Socket，如果有客户端连接则进行处理
    try {
        while (server_share_data::running_Main) {
            sockaddr_in client_address;
            socklen_t client_address_size = sizeof(client_address);
            // 类阻塞等待连接（其实这里设置了一个非阻塞情况，是为了可以用Ctrl+C把程序关掉，有连接则会跳进else条件）
            int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
            if (client_socket < 0) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    // 没有连接请求，稍等一会儿再重试
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                } else {
                    perror("accept");
                    break;
                }
            }

            std::cout << "Accepted connection from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;

            // 如有客户端连接，则会创建新线程处理客户端连接
            std::thread(server_IPC::handle_client, client_socket).detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    // 关闭主监听套接字
    close(server_socket);

    return 0;
}

