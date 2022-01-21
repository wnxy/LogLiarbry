/**
 * @file SPLog.h
 * @author XiaoYin Niu (you@domain.com)
 * @brief Header file of server library,
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef _SPLOG_H_
#define _SPLOG_H_

#ifdef _WIN64
#include <Windows.h>
#include <direct.h>
#elif __linux__
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <condition_variable>
#endif

#include <string>
#include <list>
#include <chrono>
#include <mutex>
#include <time.h>

typedef unsigned long DWORD;

enum SPLOG_LEVEL
{
    LOG_NONE,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

#ifdef _MSC_VER

#define LOG_INFO(...) SPLog::AddLogToCache(LOG_INFO, __FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__);
#define LOG_WARNING(...) SPLog::AddLogToCache(LOG_WARNING, __FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(...) SPLog::AddLogToCache(LOG_ERROR, __FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__);

#elif __GNUC__

#define LOG_INFO(...) SPLog::AddLogToCache(LOG_INFO, __FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__);
#define LOG_WARNING(...) SPLog::AddLogToCache(LOG_WARNING, __FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(...) SPLog::AddLogToCache(LOG_ERROR, __FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__);

#endif

#ifdef _WIN64
#define _PCSTR_ PCSTR
#elif __linux__
#define INVALID_HANDLE_VALUE -1
#define _PCSTR_ const char *
#define MAX_PATH 260
#define TCHAR char
#define HANDLE int
#endif

/**
 * @brief Server log encapsulation class, realize log file initialization,
 *  log level definition, log printing, etc.
 */
class SPLog
{
public:
    static bool Init(bool bTruncateLongLog, _PCSTR_ c_cLogFileName = nullptr);
    static void UnInit();

    static void AddLogToCache(long nLevel, _PCSTR_ pszFileName, _PCSTR_ pszFunctionSig, long nLineNo, _PCSTR_ pszFmt, ...);

private:
    SPLog() = delete;
    ~SPLog() = delete;

    SPLog(const SPLog &rhs) = delete;
    SPLog &operator=(const SPLog &rhs) = delete;

    static std::string GetLogTime();
    static void logOutputThread();
    static void writeLogInfo(std::string);

private:
    static bool m_bTruncateLongLog;
    static SPLOG_LEVEL m_nLogLevel; // Log level
    static HANDLE m_hLogFile;
    static std::list<std::string> logCache;
};

#endif // _SPLOG_H_