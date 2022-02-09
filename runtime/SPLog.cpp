/**
 * @file SPLog.cpp
 * @author XiaoYin Niu (you@domain.com)
 * @brief Source file of server library,
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "SPLog.h"
#include "Common.h"

#define MAX_LINE_LENGTH 256

bool SPLog::m_bTruncateLongLog = false;
SPLOG_LEVEL SPLog::m_nLogLevel = LOG_NONE;
HANDLE SPLog::m_hLogFile = INVALID_HANDLE_VALUE;
std::list<std::string> SPLog::logCache;
std::shared_ptr<std::thread> SPLog::spThread;

std::mutex mut;
std::condition_variable cv;

/**
 * @brief Log file initialier
 *
 * @param bTruncateLongLog
 * @param c_cLogFileName
 * @return true
 * @return false
 */
bool SPLog::Init(bool bTruncateLongLog, _PCSTR_ c_cLogFileName)
{
    m_bTruncateLongLog = bTruncateLongLog;
    if (c_cLogFileName == nullptr)
    {
        return false;
    }
    else
    {
        TCHAR fileDirectory[MAX_PATH];
#ifdef _WIN64
        GetCurrentDirectory(MAX_PATH, fileDirectory);

        // Log directory
        std::string logFileDirectory = format_string("%s%s", fileDirectory, "\\Log\\");
        if (_access(logFileDirectory.c_str(), 0) == -1)
        {
            _mkdir(logFileDirectory.c_str());
        }
#elif __linux__
        getcwd(fileDirectory, MAX_PATH);

        // Log directory
        std::string logFileDirectory = format_string("%s%s", fileDirectory, "/Log/");
        if (access(logFileDirectory.c_str(), F_OK) == -1)
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
#elif __linux__
        m_hLogFile = open(logFileName.c_str(), O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
#endif
        if (m_hLogFile == INVALID_HANDLE_VALUE)
        {
            return false;
        }
    }
    spThread.reset(new std::thread(std::bind(SPLog::logOutputThread)));

    return true;
}

/**
 * @brief Close log file handle
 *
 */
void SPLog::UnInit()
{
    spThread->join();

    if (m_hLogFile != INVALID_HANDLE_VALUE)
    {
#ifdef _WIN64
        CloseHandle(m_hLogFile);
#elif __linux__
        close(m_hLogFile);
#endif
        m_hLogFile = INVALID_HANDLE_VALUE;
    }
}

/**
 * @brief Get current time for print log
 *
 * @return std::string
 */
std::string SPLog::GetLogTime()
{
    auto time_now = std::chrono::system_clock::now();
    auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch()).count();
    auto seconds = time_ms / 1000;
    auto millisec = time_ms % 1000;
    struct tm *ptm = localtime(&seconds);
    std::string strTime = format_string("[%04d-%02d-%02d %02d:%02d:%02d %04d]",
                                        (int)ptm->tm_year + 1900,
                                        (int)ptm->tm_mon,
                                        (int)ptm->tm_mday,
                                        (int)ptm->tm_hour,
                                        (int)ptm->tm_min,
                                        (int)ptm->tm_sec,
                                        (int)millisec);
    return strTime;
}

/**
 * @brief 
 * 
 * @param nLevel 
 * @param pszFileName 
 * @param pszFunctionSig 
 * @param nLineNo 
 * @param pszFmt 
 * @param ... 
 */
void SPLog::AddLogToCache(long nLevel, _PCSTR_ pszFileName, _PCSTR_ pszFunctionSig, long nLineNo, _PCSTR_ pszFmt, ...)
{
    if (nLevel <= m_nLogLevel)
    {
        return;
    }
    std::string strLogLevel;
    if (nLevel == LOG_INFO)
    {
        strLogLevel = "[INFO]";
    }
    else if (nLevel == LOG_WARNING)
    {
        strLogLevel = "[WARNING]";
    }
    else if (nLevel == LOG_ERROR)
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
    strLogInfo = format_string("%s [ThreadID: %u] [%s Line: %u] [Function: %s] Message: ",
                               strLogInfo.c_str(), dwThreadID, pszFileName, nLineNo, pszFunctionSig);
    // Log message
    std::string strLogMsg;
    va_list ap;
    va_start(ap, pszFmt);
    strLogMsg = format_string(pszFmt, ap);
    va_end(ap);

    // If the log allows truncation, the long log only takes the first MAX_LINE_LENGTH characters
    if (m_bTruncateLongLog)
    {
        strLogMsg = strLogMsg.substr(0, MAX_LINE_LENGTH);
    }
    strLogInfo += strLogMsg;
    strLogInfo += "\r\n";
    {
        std::lock_guard<std::mutex> mtxLocker(mut);
        logCache.emplace_back(strLogInfo);
    }
    cv.notify_one();
}

/**
 * @brief Asynchronous write log thread
 * 
 */
void SPLog::logOutputThread()
{
    std::string strLog;
    if (m_hLogFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    while (true)
    {
        std::unique_lock<std::mutex> mtxLocker(mut);
        if (logCache.empty())
        {
            cv.wait(mtxLocker);
        }
        strLog = logCache.front();
        writeLogInfo(strLog);
        logCache.pop_front();
    }
}

/**
 * @brief Write log messages to file
 * 
 * @param strLog Log message
 */
void SPLog::writeLogInfo(std::string strLog)
{
#ifdef _WIN64
    {
        //std::lock_guard<std::mutex> mtxLocker(mut);
        SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
        DWORD dwByteWritten = 0;
        WriteFile(m_hLogFile, strLog.c_str(), strLog.length(), &dwByteWritten, NULL);
        FlushFileBuffers(m_hLogFile);
    }
#elif __linux__
    {
        //std::lock_guard<std::mutex> mtxLocker(mut);
        lseek(m_hLogFile, 0, SEEK_SET);
        int size = write(m_hLogFile, strLog.c_str(), strLog.length());
        fsync(m_hLogFile);
    }
#endif
}