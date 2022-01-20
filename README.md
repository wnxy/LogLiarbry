# LogLiarbry
# 客户端日志库

## 1. 相关文件

```
SWLog.h
SWLog.cpp
SWCommon.h
```

## 2. 日志库特点

- 使用C++编写，可以跨平台工作（支持Windows和Linux平台）
- 采用同步打印日志的方式，满足对性能要求不太高（相对服务器）的客户端软件使用
- 日志信息支持格式化输出
- 使用简单，只需要引入相关头文件
- 多线程安全

## 3. 详细信息

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
void UnInit();
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

性能测试：

