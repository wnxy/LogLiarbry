/**
 * @file SWCommon.cpp
 * @author XiaoYin Niu (you@domain.com)
 * @brief Common function of software log liarbry, source file
 * @version 0.1
 * @date 2022-01-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>

/**
 * @brief String format output to buffer
 * 
 * @param format 
 * @param args 
 * @return std::string 
 */
inline std::string format_string(const char* format, va_list args) 
{
    constexpr size_t oldlen = BUFSIZ;
    char buffer[oldlen];  // Buffers on the default stack
    va_list argscopy;
    va_copy(argscopy, args);
    size_t newlen = vsnprintf(&buffer[0], oldlen, format, args) + 1;
    newlen ++;            // Add terminator '\0'
    if(newlen > oldlen)   // Buffer on the default stack isn't enough, allocate from the heap
    {  
        std::vector<char> newbuffer(newlen);
        vsnprintf(newbuffer.data(), newlen, format, argscopy);
        return newbuffer.data();
    }
    return buffer;
}

inline std::wstring format_wstring(const wchar_t* format, va_list args)
{
    constexpr size_t oldlen = BUFSIZ;
    wchar_t buffer[oldlen];  // Buffers on the default stack
    va_list argscopy;
    va_copy(argscopy, args);
    size_t newlen = vsnprintf(&buffer[0], oldlen, format, args) + 1;
    newlen ++;            // Add terminator '\0'
    if(newlen > oldlen)   // Buffer on the default stack isn't enough, allocate from the heap
    {  
        std::vector<wchar_t> newbuffer(newlen);
        vsnprintf(newbuffer.data(), newlen, format, argscopy);
        return newbuffer.data();
    }
    return buffer;
}

/**
 * @brief String format output to buffer
 * 
 * @param format 
 * @param ... 
 * @return std::string 
 */
inline std::string format_string(const char* format, ...) 
{
    va_list args;
    va_start(args, format);
    auto s = format_string(format, args);
    va_end(args);

    return s;
}

inline std::wstring format_wstring(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    auto s = format_string(format, args);
    va_end(args);

    return s;
}