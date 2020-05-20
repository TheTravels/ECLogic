/************************ (C) COPYLEFT 2018 Merafour *************************
* File Name          : debug.c
* Author             : Merafour
* Last Modified Date : 04/17/2020
* Description        : 调试接口.
********************************************************************************
* https://merafour.blog.163.com
* merafour@163.com
* https://github.com/merafour
******************************************************************************/
#include <string.h>
//#include "../sha1/sha1.h"
//#include <board.h>
//#include <rtthread.h>
//#include "mem_malloc.h"
#include <stdio.h>
#include <stdarg.h>
#include "Periphs/uart.h"
//#include "cmsis_os.h"
#include "debug.h"

static enum log_level _log_level;

static char debug_text[256];
cache_queue Debug_RX_cache;
int debug_send(const uint8_t data[], const uint32_t _size)
{
	return usart_send(UART5, data, _size);
}
int debug_read(uint8_t buf[], const uint32_t _size)
{
	//return cache_queue_reads(&Debug_RX_cache, buf, _size);
	uint16_t index;
	//__asm("CPSID  I");
	macro_queue_read(index, buf, _size, Debug_RX_cache);
	//__asm("CPSIE  I");
	return index;
}
void debug_init(const enum log_level _level)
{
    _log_level = _level;
    init_queue(&Debug_RX_cache);
}

static inline void _logger(const void *buffer, size_t size)
{
#if 0
    size_t i;
    char* const str = (char*)buffer;
    for(i=0; i<size; i++)
    {
        fputc(str[i], stdout);
    }
#else
    debug_send(buffer, size);
#endif
}
int logger(const char* const _func, const int _line, const enum log_level _level, const char *__format, ...)
{
    // 判断日志级别
    if(_level<_log_level) return -1;
    va_list ap;
    int len;
    memset(debug_text, 0, sizeof(debug_text));
    snprintf(debug_text, sizeof(debug_text), "[%s--%d] ", _func, _line);
    len = strlen(debug_text);
    va_start(ap, __format);
    //vprintf(__format, ap);
    //snprintf(text, sizeof (text), __format, ap);
    //vsprintf(debug_text, __format, ap);
    vsnprintf(&debug_text[len], sizeof(debug_text)-len, __format, ap);
    va_end(ap);
    //_serial_debug->write(_serial_debug, 0, debug_text, strlen(debug_text));
    _logger(debug_text, strlen(debug_text));
    return 0;
}
//以十六进制方式打印数据
void print_hex(const char* const _func, const int _line, const char des[], const void* const _data, const uint16_t _size)
{
    int i;
    const char* const data = (const char*)_data;
    if(_size>0)
    {
        app_debug("[%s-%d] %s _size[%d]: \r\n", _func, _line, des, _size);
        for(i=0; i<_size; i++)
        {
            //app_debug("%02X ", data[i]&0xFF);
            app_debug("%02X", data[i]&0xFF);
            //app_debug("0x%02X, ", data[i]&0xFF);
        }
        app_debug("\r\n");
    }
}
int qDebug(const char *__format, ...)
{
	va_list ap;
    // 判断日志级别
    if(LOG_LEVEL_DEBUG<_log_level) return -1;
	memset(debug_text, 0, sizeof(debug_text));
	va_start(ap, __format);
	//vprintf(__format, ap);
	//snprintf(text, sizeof (text), __format, ap);
	vsprintf(debug_text, __format, ap);
	va_end(ap);
	//_serial_debug->write(_serial_debug, 0, debug_text, strlen(debug_text));
	debug_send((uint8_t*)debug_text, strlen(debug_text));
	//_serial_debug->write(_serial_debug, 0, "\r\n", 2);
	return 0;
}
int app_debug(const char *__format, ...)
{
	va_list ap;
    // 判断日志级别
    if(LOG_LEVEL_DEBUG<_log_level) return -1;
	memset(debug_text, 0, sizeof(debug_text));
	va_start(ap, __format);
	//vprintf(__format, ap);
	//snprintf(text, sizeof (text), __format, ap);
	vsprintf(debug_text, __format, ap);
	va_end(ap);
	//_serial_debug->write(_serial_debug, 0, debug_text, strlen(debug_text));
	debug_send((uint8_t*)debug_text, strlen(debug_text));
	//_serial_debug->write(_serial_debug, 0, "\r\n", 2);
	return 0;
}





