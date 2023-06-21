// SocketJSONDLL.h

#ifndef SOCKETJSONDLL_H
#define SOCKETJSONDLL_H

#ifdef SOCKETJSONDLL_EXPORTS
#define SOCKETJSONDLL_API __declspec(dllexport)
#else
#define SOCKETJSONDLL_API __declspec(dllimport)
#endif

#include <string>

extern "C" __declspec(dllexport) std::string SendData(const std::string& path);
extern "C" __declspec(dllexport) int Add(int a, int b);

#endif