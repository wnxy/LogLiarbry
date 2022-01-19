#include <iostream>
#include "SWLog.h"

int main()
{
	SWLog::Init(true, false, "test.txt");
	LOG_INFO("This is test message!");
	LOG_WARNING("这是一条测试警告日志！");
	LOG_ERROR("这是一条测试错误日志！");
	SWLog::UnInit();
	std::cout << "Hello CMake." << std::endl;
	return 0;
}