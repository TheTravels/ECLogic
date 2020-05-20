/**
  ******************************************************************************
  * File Name          : wave_frame.h,波形数据帧
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#ifndef _WAVE_FRAME_H_
#define _WAVE_FRAME_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>

/*
* 数据接收关键两点，buf要小、接收要快，实测 2017字节 200ms频率收发连续测试 445757字节无数据丢失。
 */
#define WAVE_FRAME_UART    (128)
//#define WAVE_FRAME_UART    (512)
struct wave_frame_uart{
	uint8_t bits[16];    // 发送波形,二进制
	uint8_t _txi;       // 发送bit索引
	uint8_t _txl;       // 发送bit位数
	uint8_t txi;        // 发送byte索引
	uint8_t txl;        // 发送byte数
	uint8_t tx[WAVE_FRAME_UART];
};

//extern struct wave_frame_uart _wave_frame_uart;
extern void wave_frame_uart_Oversampling(void);
extern void _wave_frame_uart_Init(const uint16_t baud);
extern uint16_t wave_frame_uart_send(const uint8_t data[], const uint16_t _len);

#endif //_WAVE_FRAME_H_
