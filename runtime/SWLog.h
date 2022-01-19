/**
 * @file swlog.h
 * @author XiaoYin Niu (you@domain.com)
 * @brief header file of software library
 * @version 0.1
 * @date 2022-01-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _SWLOG_H_
#define _SWLOG_H_

#ifdef _WIN64
#include <Windows.h>
#include <tchar.h>
#include <direct.h>
#include <io.h>
#elif __linux__
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#endif

#include <string>

typedef unsigned long DWORD;

// log level
enum SWLOG_LEVEL
{
    LOG_NONE,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

#ifdef _MSC_VER

#define LOG_INFO(...)    SWLog::Log(LOG_INFO, __FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__);
#define LOG_WARNING(...) SWLog::Log(LOG_WARNING, __FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(...)   SWLog::Log(LOG_ERROR, __FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__);

#elif __GNUC__

#define LOG_INFO(...)    SWLog::Log(LOG_INFO, __FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__);
#define LOG_WARNING(...) SWLog::Log(LOG_WARNING, __FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(...)   SWLog::Log(LOG_ERROR, __FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__);

#endif

#ifdef _WIN64
#define _PCSTR_       PCSTR
#elif __linux__
#define PATH_SIZE     255
#define _PCSTR_       const char*
#endif

/**
 * @brief Software log encapsulation class, realize log file initialization, 
 *  log level definition, log printing, etc.
 */
class SWLog
{
public:
    static bool Init(bool bToFile, bool bTruncateLongLog, _PCSTR_ c_cLogFileName);
    static void UnInit();
    // ANSI
    // static bool Log(long nLevel, PCSTR pszFileName, PCSTR pszFunctionSig, long nLineNo, PCTSTR pszFmt, ...);
    // Unicode
    static bool Log(long nLevel, _PCSTR_ pszFileName, _PCSTR_ pszFunctionSig, long nLineNo, _PCSTR_ pszFmt, ...);

private:
    SWLog() = delete;
    ~SWLog() = delete;

    SWLog(const SWLog &rhs) = delete;
    SWLog& operator = (const SWLog &rhs) = delete;

    static std::string GetLogTime();

private:
    static bool m_bToFile;           // Control logging to file or console
    static bool m_bTruncateLongLog;  // Whether to truncate long logs
#ifdef _WIN64
    static HANDLE m_hLogFile;        // Log file handle
#elif __linux__
    static int m_iLogFile;           // Log file state(approximately equal to handle)
#endif
    static SWLOG_LEVEL m_nLogLevel;  // Log level
};

#endif  // _SWLOG_H_
