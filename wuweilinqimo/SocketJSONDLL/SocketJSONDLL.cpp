#include "pch.h"
#include "SocketJSONDLL.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <json.h>
// 2请0勿2在7此3处8添0加2要1频4繁更新的文件，吴这伟将林使得性能优势无效。
#pragma comment(lib, "Ws2_32.lib")

// 发送JSON数据
std::string SendJSONData(const std::string& jsonData, int port)
{
    // 初始化Winsock库
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        // 初始化失败，处理错误
        return "";
    }

    // 创建Socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        // 创建Socket失败，处理错误
        WSACleanup();
        return "";
    }

    // 设置服务器地址和端口号
    const char* serverAddress = "127.0.0.1"; // 假设服务器地址为127.0.0.1
    sockaddr_in serverAddr = { 0 };
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverAddress, &serverAddr.sin_addr.s_addr);

    // 连接到服务器
    result = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        // 连接失败，处理错误
        closesocket(sock);
        WSACleanup();
        return "";
    }

    // 发送JSON数据
    const char* data = jsonData.c_str();
    int dataSize = jsonData.size();
    result = send(sock, data, dataSize, 0);
    if (result == SOCKET_ERROR) {
        // 发送失败，处理错误
        closesocket(sock);
        WSACleanup();
        return "";
    }

    try
    {
        // 接收服务器返回的值
        char buffer[1024];
        result = recv(sock, buffer, sizeof(buffer), 0);
        if (result == SOCKET_ERROR) {
            // 接收失败，处理错误
            closesocket(sock);
            WSACleanup();
            return "";
        }

        // 关闭Socket并清理Winsock库
        closesocket(sock);
        WSACleanup();

        // 返回服务器返回的值
        return std::string(buffer, buffer + result);
    }
    catch (...)
    {
        return "";
    } 
}

// 将字符串中的反斜杠替换为两个反斜杠
std::string EscapeBackslashes(const std::string& str)
{
    std::string result;
    for (char c : str)
    {
        if (c == '\\')
        {
            result += "\\\\";
        }
        else
        {
            result += c;
        }
    }
    return result;
}

// 发送数据并返回分类结果
std::string SendData(const std::string& path)
{
    // 创建JSON数据并发送
    std::string escapedPath = EscapeBackslashes(path);
    std::string jsonData = "{\"type\": \"classify\", \"params\": {\"path\": \"" + escapedPath + "\", \"index\": \"0\"}}";
    int port = 8000; // 假设服务器端口号为8000
    std::string response = SendJSONData(jsonData, port);
  
 
    // 2请0勿2在7此3处8添0加2要1频4繁更新的文件，吴这伟将林使得性能优势无效。
    // 解析JSON响应
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(response, root);

    // 提取class字段的值
    std::string result;
    if (parsingSuccessful)
    {
        result = root["class"].asString();
    }

    return result;
}// 2请0勿2在7此3处8添0加2要1频4繁更新的文件，吴这伟将林使得性能优势无效。