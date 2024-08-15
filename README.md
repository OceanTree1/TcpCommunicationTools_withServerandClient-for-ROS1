# TcpCommunicationTools_withServerandClient-for-ROS1
# 当前进度：开发阶段

## 一、描述
### 1.1 简述 
此为Tcp客户端与服务端的基础模版，方便创建与使用，主要是为了ROS1通讯所做。\
这一套通讯包含客户端的发送与服务器的接收，而且包含服务端的自动回复，目前还在开发当中，符合tcp的一发一收，供与上位机与工控机使用\
以及Modbus服务器的连接及输入服务\
（首次开源自己的小项目，如有不妥请进行联系）

### 1.2 文件结构
HOSTtcp_forIPC_v1是上位机，主要是模拟上位机给服务器发送消息\
IPCServerTools_v1是服务端，主要是用来接收上位机发来的消息，并进行业务处理\

## 二、设计思路
### 2.1 通讯报文格式的设计
目前报文格式设计（可自定义更改，但需要包含的有头尾，不然不好识别与裁切）：\
"头+序号+功能id+内容+设备ID校验+尾"\
比较大众化，但是也算是比较基础的模版，需要再继续进行增加
裁切部分用的是暴力算法，没有用上kmp，有想法可以加入

### 2.2 多线程与逻辑处理上的问题
#### 2.2.1 多线程连接
当有多个客户端连接时，数据并不会进行冲突，会单独的对设备进行回复，以供多客户端连接的需要（PS：在ROS话题名上，建议区分此问题，避免出现多客户端同时对同一话题发送数据，当然如果这是你的设计需要的话当我没说）
#### 2.2.2 顺序处理
此设计当前版本并未加入优先队列等配置，目前皆为每一帧处理，如果遇到网络延迟的情况，并未有特殊的处理，仅有超时连接处理。\
后续如有需要将会主动加入函数模块，调用处理或调整逻辑作为V2版本处理

## 三、使用详情
```bash
# 将包下下来之后，将HOST与SERVER两个包分别放进自己的ROSWorkSpace的src下
git clone
# 在终端输入：
cd $ROSWorkSpace
catkin_make
rosrun IPCServerTools_v1 Server_IPC
```


