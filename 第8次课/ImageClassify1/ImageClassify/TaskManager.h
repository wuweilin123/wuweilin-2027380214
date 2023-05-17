#pragma once
#include <string>

enum IMAGE_TYPE
{
	NORMAL_IMAGE = 6,
	FG_IMAGE =10,
	NG_IMAGE ,
	AG_IMAGE 
};

class CTaskManager
{
public:
	CTaskManager();
	~CTaskManager();

	IMAGE_TYPE AddTask(std::wstring strFilePath);
	void AddDirTask(std::wstring strDirPath);
};

