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
 * @brief Log file initialier.
 * If the first parameter specifies the output log to the console, the third parameter can be omitted.
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
SWLog::Init(true, false, "test.txt");     // 使用前需要初始化日志模块
LOG_INFO("This is test message!");        // LOG_INFO LOG_WARNING LOG_ERROR使用方式相同
SWLog::UnInit();                          // 使用后建议卸载日志模块

// 日志打印的格式化输出与printf()函数相同
for (int i = 0; i < RECORDS; ++i) 
{
    LOG_INFO("Number %d,simple log message with 0 parameters", i);
}
```

日志打印结果：

```
[2022-01-20 10:15:25 0796] [INFO] [ThreadID: 3449112384] [sample/main.cpp Line: 37] [Function: void runBenchmark()] Message: Number 0,simple log message with 0 parameters
[2022-01-20 10:15:25 0873] [INFO] [ThreadID: 3449112384] [sample/main.cpp Line: 37] [Function: void runBenchmark()] Message: Number 1,simple log message with 0 parameters
[2022-01-20 10:15:25 0940] [INFO] [ThreadID: 3449112384] [sample/main.cpp Line: 37] [Function: void runBenchmark()] Message: Number 2,simple log message with 0 parameters
[2022-01-20 10:15:26 0006] [INFO] [ThreadID: 3449112384] [sample/main.cpp Line: 37] [Function: void runBenchmark()] Message: Number 3,simple log message with 0 parameters
[2022-01-20 10:15:26 0073] [INFO] [ThreadID: 3449112384] [sample/main.cpp Line: 37] [Function: void runBenchmark()] Message: Number 4,simple log message with 0 parameters
```

运行sample测试：

```makefile
# 在项目文件夹下运行
make           # 编译文件        
make clean     # 清空编译结果
make run       # 运行示例
```

