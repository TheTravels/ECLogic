/************************ (C) COPYLEFT 2018 Merafour *************************
* File Name          : debug.h
* Author             : Merafour
* Last Modified Date : 04/17/2020
* Description        : 调试接口.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

// 定义日志级别
enum log_level{
    LOG_LEVEL_TRACE    = 0x00,
    LOG_LEVEL_DEBUG    = 0x02,
    LOG_LEVEL_INFO     = 0x04,
    LOG_LEVEL_WARN     = 0x06,
    LOG_LEVEL_ERROR    = 0x08,
    LOG_LEVEL_FATAL    = 0x0A,
    LOG_LEVEL_SYSERR   = 0x0C,
    LOG_LEVEL_SYSFATAL = 0x0E,
};

// debug 模块初始化
extern void debug_init(const enum log_level _level);
// 日志,参数:    函数名,行号,变参
extern int logger(const char* const _func, const int _line, const enum log_level _level, const char *__format, ...);
#define Logger(_level,fmt, ...) logger(__func__, __LINE__, _level, fmt, ##__VA_ARGS__)
// 调试接口
extern int qDebug(const char *__format, ...);
extern int app_debug(const char *__format, ...);

extern void print_hex(const char* const _func, const int _line, const char des[], const void* const _data, const uint16_t _size);

  
#ifdef __cplusplus
}
#endif

#endif  // _RECORE_H_
