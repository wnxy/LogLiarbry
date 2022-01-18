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

#define MAX_LINE_LENGTH 256

bool SWLog::m_bToFile = false;
bool SWLog::m_bTruncateLongLog = false;
#ifdef _WIN64
HANDLE SWLog::m_hLogFile = INVALID_HANDLE_VALUE;
#elif __linux__
int SWLog::m_iLogFile = -1;
#endif
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
bool SWLog::Init(bool bToFile, bool bTruncateLongLog, _PCSTR_ c_cLogFileName)
{
    m_bToFile = bToFile;
    m_bTruncateLongLog = bTruncateLongLog;

    if(c_cLogFileName == nullptr)
    {
        return false;
    }
#ifdef _WIN64
    TCHAR fileDirectory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, fileDirectory);
#elif __linux__
    char fileDirectory[PATH_SIZE];
    getcwd(fileDirectory, PATH_SIZE);
#endif
    // Log directory
    std::string logFileDirectory = format_string("%s%s", fileDirectory, "\\Log\\");
#ifdef _WIN64
    if(_access(logFileDirectory.c_str(), 0) == -1)
    {
        _mkdir(logFileDirectory.c_str());
    }
#elif __linux__
    if(access(logFileDirectory.c_str(), F_OK) == -1)
    {
        mkdir(logFileDirectory.c_str(), S_IRWXU);
    }
#endif
    std::string logFileName = format_string("%s%s", logFileDirectory.c_str(), c_cLogFileName);
#ifdef _WIN64
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
#elif __linux__
    m_iLogFile = open(logFileName.c_str(), O_APPEND | O_CREAT, S_IRWXU);
    if(m_iLogFile == -1)
    {
        return false;
    }
#endif
    return true;
}

/**
 * @brief Close log file handle
 * 
 */
void SWLog::UnInit()
{
#ifdef _WIN64
    if(m_hLogFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hLogFile);
        m_hLogFile = INVALID_HANDLE_VALUE;
    }
#elif __linux__
    if(m_iLogFile != -1)
    {
        close(m_iLogFile);
        m_iLogFile = -1;
    }
#endif
}

/**
 * @brief Get current time for print log
 * 
 * @param strTime Time string
 * @param nTimeLength Length of time string
 */
std::string SWLog::GetLogTime()
{
#ifdef _WIN64
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
#elif __linux__
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // millisecond
    int millis = tv.tv_usec / 1000;
    struct tm nowTime;
    localtime_r(&tv.tv_sec, &nowTime);
    char buffer[80] = {0};
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &nowTime);
    std::string strTime = format_string("[%s %04d]", buffer, millis);
#endif
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
bool SWLog::Log(long nLevel, _PCSTR_ pszFileName, _PCSTR_ pszFunctionSig, long nLineNo, _PCSTR_ pszFmt, ...)
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
#ifdef _WIN64
    DWORD dwThreadID = GetCurrentThreadId();
#elif __linux__
    DWORD dwThreadID = pthread_self();
#endif
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
#ifdef _WIN64
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
#elif __linux__
    if(m_bToFile)
    {
        if(m_iLogFile == -1)
        {
            return false;
        }
        lseek(m_iLogFile, 0, SEEK_SET);
        int size = write(m_iLogFile, strLogInfo.c_str(), strLogInfo.length());
        fsync(m_iLogFile);
        return true;
    }

    // Output the log to console in software release state
    printf("%s", strLogInfo.c_str());
#endif

    return true;
}