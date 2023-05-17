#include "pch.h"
#include "TaskManager.h"

CTaskManager::CTaskManager()
{

}

CTaskManager::~CTaskManager()
{

}

IMAGE_TYPE CTaskManager::AddTask(std::wstring strFilePath)
{
	return IMAGE_TYPE::AG_IMAGE;
}

void CTaskManager::AddDirTask(std::wstring strDirPath)
{
	//1.for循环来枚举目录下的所有文件


	//2.每个文件都调用AddTask
}
