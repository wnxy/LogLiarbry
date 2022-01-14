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

#include <string>

/**
 * @brief String format output
 * 
 * @param format 
 * @param args 
 * @return std::string 
 */
inline std::string format_string(const char* format, va_list args);

inline std::wstring format_wstring(const wchar_t* format, va_list args);

/**
 * @brief String format output
 * 
 * @param format 
 * @param ... 
 * @return std::string 
 */
inline std::string format_string(const char* format, ...);

inline std::wstring format_wstring(const wchar_t* format, ...);

#endif  // _SWCOMMON_H_