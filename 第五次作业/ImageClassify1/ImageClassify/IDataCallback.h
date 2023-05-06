#pragma once
#include <string>

class IDataCallback
{
public:
	virtual void OnDataCallback(std::string strData) = 0 ;
};