# SoftwareLogLiarbry
C++ 软件日志库，可以在Windows和Linux平台下稳定工作，采用同步打印日志的方式，满足软件对日志打印的需求，使用时只需要引入相关头文件。

使用前需要初始化，API为：

```c++
/**
 * @brief Log file initialier
 * 
 * @param bToFile Define logging to file or console
 * @param bTruncateLongLog Truncate long log true/false
 * @param c_cLogFileName Log filename
 * @return true 
 * @return false 
 */
bool Init(bool bToFile, bool bTruncateLongLog, _PCSTR_ c_cLogFileName);
```

日志打印完毕后建议释放资源，API为：

```c++
void UnInit()
```

日志等级分为：

```
LOG_INFO(...)   
LOG_WARNING(...) 
LOG_ERROR(...)  
```

使用实例：

```c++
#include "SWLog.h"

int main()
{
  SWLog::Init(true, false, "test.txt");
  LOG_INFO("This is test message!");
  LOG_WARNING("这是一条测试警告日志！");
  LOG_ERROR("这是一条测试错误日志！");
  SWLog::UnInit();
  return 0;
}
```

日志打印结果：

```
[2022-01-18 15:26:16 0811] [INFO] [ThreadID: 3501497728] [main.cpp Line: 7] [Function: int main()] Message: This is test message!
[2022-01-18 15:26:16 0873] [WARNING] [ThreadID: 3501497728] [main.cpp Line: 8] [Function: int main()] Message: 这是一条测试警告日志！
[2022-01-18 15:26:16 0931] [ERROR] [ThreadID: 3501497728] [main.cpp Line: 9] [Function: int main()] Message: 这是一条测试错误日志！
```

