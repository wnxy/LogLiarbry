/**
 * @file SWLog.cpp
 * @author XiaoYin Niu (you@domain.com)
 * @brief Source file of software library
 * @version 0.1
 * @date 2022-01-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "SWLog.h"
#include "SWCommon.h"
#include <tchar.h>
#include <direct.h>
#include <io.h>

#define MAX_LINE_LENGTH 256

bool SWLog::m_bToFile = false;
bool SWLog::m_bTruncateLongLog = false;
HANDLE SWLog::m_hLogFile = INVALID_HANDLE_VALUE;
SWLOG_LEVEL SWLog::m_nLogLevel = LOG_NONE;

/**
 * @brief Log file initialier
 * 
 * @param bToFile Define logging to file or console
 * @param bTruncateLongLog Truncate long log true/false
 * @param c_cLogFileName Log filename
 * @return true 
 * @return false 
 */
bool SWLog::Init(bool bToFile, bool bTruncateLongLog, PCSTR c_cLogFileName)
{
    m_bToFile = bToFile;
    m_bTruncateLongLog = bTruncateLongLog;

    if(c_cLogFileName == nullptr)
    {
        return false;
    }
    TCHAR fileDirectory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, fileDirectory);
    // Log directory
    //TCHAR logFileDirectory[MAX_PATH];
    std::string logFileDirectory = format_string("%s%s", fileDirectory, "\\Log\\");
    //strcpy(logFileDirectory, strcat(fileDirectory, "\\Log\\%s"));
    if(_access(logFileDirectory.c_str(), 0) == -1)
    {
        _mkdir(logFileDirectory.c_str());
    }
    std::string logFileName = format_string("%s%s", logFileDirectory.c_str(), c_cLogFileName);
    m_hLogFile = CreateFile(logFileName.c_str(), 
        GENERIC_READ | GENERIC_WRITE, 
        FILE_SHARE_READ, 
        NULL, 
        OPEN_ALWAYS, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL);
    if(m_hLogFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    return true;
}

/**
 * @brief Close log file handle
 * 
 */
void SWLog::UnInit()
{
    if(m_hLogFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hLogFile);
        m_hLogFile = INVALID_HANDLE_VALUE;
    }
}

/**
 * @brief Get current time for print log
 * 
 * @param strTime Time string
 * @param nTimeLength Length of time string
 */
std::string SWLog::GetLogTime()
{
    SYSTEMTIME st = { 0 };
    GetLocalTime(&st);
    // Output format control: %04d Output in four-digit digital format, zeros are added to the left of less than 4 digits
    std::string strTime = format_string("[%04d-%02d-%02d %02d:%02d:%02d %04d]", 
        st.wYear, 
        st.wMonth, 
        st.wDay, 
        st.wHour, 
        st.wMinute, 
        st.wSecond, 
        st.wMilliseconds);
    return strTime;
}

/**
 * @brief 
 * 
 * @param nLevel Log level
 * @param pszFileName Current filename
 * @param pszFunctionSig Current function name
 * @param nLineNo Current lineNo
 * @param pszFmt Log message
 * @param ... 
 * @return true 
 * @return false 
 */
bool SWLog::Log(long nLevel, PCSTR pszFileName, PCSTR pszFunctionSig, long nLineNo, PCSTR pszFmt, ...)
{
    if(nLevel <= m_nLogLevel)
    {
        return false;
    }
    std::string strLogLevel;
    if(nLevel == LOG_INFO)
    {
        strLogLevel = "[INFO]";
    }
    else if(nLevel == LOG_WARNING)
    {
        strLogLevel = "[WARNING]";
    }
    else if(nLevel == LOG_ERROR)
    {
        strLogLevel = "[ERROR]";
    }
    std::string strLogInfo = format_string("%s %s", GetLogTime().c_str(), strLogLevel.c_str());
    // Capture current thread ID
    DWORD dwThreadID = GetCurrentThreadId();
    strLogInfo = format_string("%s [ThreadID: %u] [%s Line: %u] [Function: %s]", 
        strLogInfo.c_str(), dwThreadID, pszFileName, nLineNo, pszFunctionSig);
    // Log message
    std::string strLogMsg;
    //va_list ap;
    //va_start(ap, pszFmt);
    strLogMsg = format_string(" Message: %s", pszFmt);
    //va_end(ap);

    // If the log allows truncation, the long log only takes the first MAX_LINE_LENGTH characters
    if(m_bTruncateLongLog)
    {
        strLogMsg = strLogMsg.substr(0, MAX_LINE_LENGTH);
    }
    strLogInfo += strLogMsg;
    strLogInfo += "\r\n";
    if(m_bToFile)
    {
        if(m_hLogFile == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
        DWORD dwByteWritten = 0;
        WriteFile(m_hLogFile, strLogInfo.c_str(), strLogInfo.length(), &dwByteWritten, NULL);
        FlushFileBuffers(m_hLogFile);
        return true;
    }
    // Output the log to console in software release state
    OutputDebugStringA(strLogInfo.c_str());

    return true;
}