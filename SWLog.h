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

#include <Windows.h>

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

/**
 * @brief Software log encapsulation class, realize log file initialization, 
 *  log level definition, log printing, etc.
 */
class SWLog
{
public:
    static bool Init(bool bToFile, bool bTruncateLongLog, const char* c_cLogFileName);
    static void UnInit();
    // Don't output the thread ID number and the function signature and line number
    static bool Log(long nLevel, PCTSTR pszFmt, ...);
    // ANSI
    // static bool Log(long nLevel, PCSTR pszFileName, PCSTR pszFunctionSig, long nLineNo, PCTSTR pszFmt, ...);
    // Unicode
    static bool Log(long nLevel, PCSTR pszFileName, PCSTR pszFunctionSig, long nLineNo, PCSTR pszFmt, ...);

private:
    SWLog() = delete;
    ~SWLog() = delete;

    SWLog(const SWLog &rhs) = delete;
    SWLog& operator = (const SWLog &rhs) = delete;

    static void GetLogTime(char *strTime, int nTimeLength);

private:
    static bool m_bToFile;           // Control logging to file or console
    static bool m_bTruncateLongLog;  // Whether to truncate long logs
    static HANDLE m_hLogFile;        // Log file handle
    static SWLOG_LEVEL m_nLogLevel;  // Log level
};

#endif  // _SWLOG_H_
