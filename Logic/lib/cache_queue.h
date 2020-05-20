/**
  ******************************************************************************
  * File Name          : terminal.h
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
//#include "usart.h"

/*
* 数据接收关键两点，buf要小、接收要快，实测 2017字节 200ms频率收发连续测试 445757字节无数据丢失。
 */
#define CACHE_QUEUE_LEN    (512+1024+1024)
//#define CACHE_QUEUE_LEN    (512)
typedef struct{
		uint16_t index_r;
		uint16_t index_w;
		//uint16_t w_tmp;
		//uint16_t r_tmp;
		uint8_t buf[CACHE_QUEUE_LEN];
}cache_queue;

static __inline void init_queue(cache_queue* const _queue)
{ 
		memset(_queue, 0, sizeof(cache_queue)); 
		_queue->index_r = sizeof(_queue->buf)-1;
}
#define macro_queue_read(index,buf,_size,_queue)  do{ \
			memset(buf, 0, _size); \
			for(index=0; index<_size; index++) \
			{ \
				  uint16_t r_tmp; \
					r_tmp = _queue.index_r+1; \
					if(r_tmp>=sizeof(_queue.buf)) r_tmp=0; \
					if(r_tmp == _queue.index_w) break;  /* empty */ \
					buf[index] = _queue.buf[r_tmp]; \
					_queue.index_r = r_tmp; \
			} \
}while(0);
#if 0
#define macro_queue_write(_byte,_queue)  do{ \
			_queue.w_tmp = _queue.index_w+1; \
			if(_queue.w_tmp>=sizeof(_queue.buf)) _queue.w_tmp=0; \
			_queue.buf[_queue.w_tmp] = _byte; \
			if(_queue.w_tmp != _queue.index_r) _queue.index_w=_queue.w_tmp; \
}while(0);
#else
#define macro_queue_write_(_byte,_queue)  do{ \
			uint16_t w_tmp; \
			_queue.buf[_queue.index_w] = _byte; \
			w_tmp = _queue.index_w+1; \
			if(w_tmp>=sizeof(_queue.buf)) w_tmp=0; \
			if(w_tmp != _queue.index_r) _queue.index_w=w_tmp; \
}while(0);
//#else
#define macro_queue_write(_byte,_queue)  do{ \
			if(_queue.index_w != _queue.index_r) _queue.buf[_queue.index_w++] = _byte; \
			if(_queue.index_w>=sizeof(_queue.buf)) _queue.index_w=0; \
}while(0);
#endif

static __inline int cache_queue_write(cache_queue* const _queue, const uint8_t _byte)  
{ 
#if 0
			uint16_t w_tmp;
			_queue->buf[_queue->index_w] = _byte; 
			w_tmp = _queue->index_w+1; 
			if(w_tmp>=sizeof(_queue->buf)) w_tmp=0; 
			if(w_tmp != _queue->index_r) 
			{
				_queue->index_w=w_tmp;
				return 1;
			}				
			return 0;
#else
			if(_queue->index_w != _queue->index_r)_queue->buf[_queue->index_w++] = _byte; 
			if(_queue->index_w>=sizeof(_queue->buf)) _queue->index_w=0; 
			return 1;
#endif
}
static __inline int cache_queue_writes(cache_queue* const _queue, const uint8_t _byte[], const uint16_t _size)  
{ 
			uint16_t w_tmp;
			uint16_t len;
	for(len=0; len<_size; len++)
	{
		_queue->buf[_queue->index_w] = _byte[len]; 
		w_tmp = _queue->index_w+1; 
			if(w_tmp>=sizeof(_queue->buf)) w_tmp=0; 
			if(w_tmp == _queue->index_r) 
			{
				break;
			}	
	}		
			return len;
}
static __inline uint16_t cache_queue_read_bak(cache_queue* _cache, uint8_t buf[], const uint16_t buf_size)
{
		uint16_t index=0;
	    uint16_t r_tmp;
		memset(buf, 0, buf_size);
#if 0
			//memset(&buf, 0, sizeof(buf));
			for(index=0; index<buf_size; index++)
			{
					cache.r_tmp = cache.index_r+1;
					if(cache.r_tmp>=sizeof(cache.buf)) cache.r_tmp=0;
					if(cache.r_tmp == cache.index_w) break;  // empty
					buf[index] = cache.buf[cache.r_tmp];
					cache.index_r = cache.r_tmp;
			}
#else
		for(index=0; index<buf_size; index++)
		{
				r_tmp = _cache->index_r+1;
				if(r_tmp>=sizeof(_cache->buf)) r_tmp=0;
				if(r_tmp == _cache->index_w) break;  // empty
				buf[index] = _cache->buf[r_tmp];
				_cache->index_r = r_tmp;
		}
#endif
		return index;
}
static __inline uint16_t cache_queue_reads(cache_queue* _cache, uint8_t buf[], const uint16_t buf_size)
{
		uint16_t index=0;
	  //uint16_t r_tmp;
		memset(buf, 0, buf_size);
		for(index=0; index<buf_size; index++)
		{
				/*buf[index] = _cache->buf[_cache->index_r];
				r_tmp = _cache->index_r+1;
				if(r_tmp>=sizeof(_cache->buf)) r_tmp=0;
				if(_cache->index_r == _cache->index_w) break;  // empty
				_cache->index_r = r_tmp;*/
				if(_cache->index_r == _cache->index_w) break;  // empty
				buf[index] = _cache->buf[_cache->index_r++];
				if(_cache->index_r>=sizeof(_cache->buf)) _cache->index_r=0;
		}
		return index;
}


static __inline int cache_queue_read(cache_queue* _cache, uint8_t* _byte)
{
		int ret=-1;
	  uint16_t r_tmp;
		//__asm("CPSID  I");
		r_tmp = _cache->index_r+1;
		if(r_tmp>=sizeof(_cache->buf)) r_tmp=0;
		if(r_tmp != _cache->index_w)
		{
				*_byte = _cache->buf[r_tmp];
				_cache->index_r = r_tmp;
				ret = 0;
		}
		//__asm("CPSIE  I");
		return ret;
}

#endif //_TERMINAL_H_
