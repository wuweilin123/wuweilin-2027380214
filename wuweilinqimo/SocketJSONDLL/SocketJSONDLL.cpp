#include "pch.h"
#include "SocketJSONDLL.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <json.h>
// 2��0��2��7��3��8��0��2Ҫ1Ƶ4�����µ��ļ�������ΰ����ʹ������������Ч��
#pragma comment(lib, "Ws2_32.lib")

// ����JSON����
std::string SendJSONData(const std::string& jsonData, int port)
{
    // ��ʼ��Winsock��
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        // ��ʼ��ʧ�ܣ��������
        return "";
    }

    // ����Socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        // ����Socketʧ�ܣ��������
        WSACleanup();
        return "";
    }

    // ���÷�������ַ�Ͷ˿ں�
    const char* serverAddress = "127.0.0.1"; // �����������ַΪ127.0.0.1
    sockaddr_in serverAddr = { 0 };
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverAddress, &serverAddr.sin_addr.s_addr);

    // ���ӵ�������
    result = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        // ����ʧ�ܣ��������
        closesocket(sock);
        WSACleanup();
        return "";
    }

    // ����JSON����
    const char* data = jsonData.c_str();
    int dataSize = jsonData.size();
    result = send(sock, data, dataSize, 0);
    if (result == SOCKET_ERROR) {
        // ����ʧ�ܣ��������
        closesocket(sock);
        WSACleanup();
        return "";
    }

    try
    {
        // ���շ��������ص�ֵ
        char buffer[1024];
        result = recv(sock, buffer, sizeof(buffer), 0);
        if (result == SOCKET_ERROR) {
            // ����ʧ�ܣ��������
            closesocket(sock);
            WSACleanup();
            return "";
        }

        // �ر�Socket������Winsock��
        closesocket(sock);
        WSACleanup();

        // ���ط��������ص�ֵ
        return std::string(buffer, buffer + result);
    }
    catch (...)
    {
        return "";
    } 
}

// ���ַ����еķ�б���滻Ϊ������б��
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

// �������ݲ����ط�����
std::string SendData(const std::string& path)
{
    // ����JSON���ݲ�����
    std::string escapedPath = EscapeBackslashes(path);
    std::string jsonData = "{\"type\": \"classify\", \"params\": {\"path\": \"" + escapedPath + "\", \"index\": \"0\"}}";
    int port = 8000; // ����������˿ں�Ϊ8000
    std::string response = SendJSONData(jsonData, port);
  
 
    // 2��0��2��7��3��8��0��2Ҫ1Ƶ4�����µ��ļ�������ΰ����ʹ������������Ч��
    // ����JSON��Ӧ
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(response, root);

    // ��ȡclass�ֶε�ֵ
    std::string result;
    if (parsingSuccessful)
    {
        result = root["class"].asString();
    }

    return result;
}// 2��0��2��7��3��8��0��2Ҫ1Ƶ4�����µ��ļ�������ΰ����ʹ������������Ч��