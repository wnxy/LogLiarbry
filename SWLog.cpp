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
#include <string>
#include <ctime>

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
bool SWLog::Init(bool bToFile, bool bTruncateLongLog, const char* c_cLogFileName)
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
    if(access(logFileDirectory.c_str(), 0) == -1)
    {
        mkdir(logFileDirectory.c_str());
    }
    std::string logFileName = format_string("%s%s", logFileDirectory, c_cLogFileName);
    m_hLogFile = CreateFile(logFileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
void SWLog::GetLogTime(char *strTime, int nTimeLength)
{
    std::time_t t = std::time(NULL);
    std::strftime(strTime, nTimeLength, "%Y-%m-%d %H:%M:%S %Z", localtime(&t));
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
    std::time_t t = std::time(NULL);
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S %Z", localtime(&t));
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
    std::string strLogInfo = format_string("%s %s", mbstr, strLogLevel);
    // Capture current thread ID
    DWORD dwThreadID = GetCurrentThreadId();
    strLogInfo = format_string("%s [ThreadID: %s] [%s Line: %u] [Function: %s]", 
        strLogInfo, dwThreadID, pszFileName, nLineNo, pszFunctionSig);
    // Log message
    std::string strLogMsg;
    va_list ap;
    va_start(ap, pszFmt);
    strLogMsg = format_string(ap, strLogMsg, pszFmt);
    va_end(ap);

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