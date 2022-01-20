/**
 * @file SWCommon.h
 * @author XiaoYin Niu (you@domain.com)
 * @brief Common function of software log liarbry, header file
 * @version 0.1
 * @date 2022-01-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef _SWCOMMON_H_
#define _SWCOMMON_H_

#include <cstdarg>
#include <vector>
#include <string>

/**
 * @brief String format output
 *
 * @param format
 * @param args
 * @return std::string
 */
inline std::string format_string(const char *format, va_list args)
{
    constexpr size_t oldlen = BUFSIZ;
    char buffer[oldlen]; // Buffers on the default stack
    va_list argscopy;
    va_copy(argscopy, args);
    size_t newlen = vsnprintf(&buffer[0], oldlen, format, args) + 1;
    newlen++;            // Add terminator '\0'
    if (newlen > oldlen) // Buffer on the default stack isn't enough, allocate from the heap
    {
        std::vector<char> newbuffer(newlen);
        vsnprintf(newbuffer.data(), newlen, format, argscopy);
        return newbuffer.data();
    }
    return buffer;
}

/**
 * @brief String format output
 *
 * @param format
 * @param ...
 * @return std::string
 */
inline std::string format_string(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    auto s = format_string(format, args);
    va_end(args);

    return s;
}

#endif // _SWCOMMON_H_