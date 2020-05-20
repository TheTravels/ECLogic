/**
  ******************************************************************************
  * File Name          : wave_frame.c,波形数据帧
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "wave_frame.h"
#include <string.h>
#include <stdio.h>
#include "main.h"

static struct wave_frame_uart _wave_frame_uart = {
	.bits = 0,
	._txi = 0,
	._txl = 0,
	.txi = 0,
	.txl = 48,
	.tx = "static struct wave_frame_uart _wave_frame_uart\r\n",
};

// Oversampling:过采样倍数
static inline uint8_t wave_frame_bit(uint8_t bits[8], uint8_t pos, const uint8_t val, const uint8_t Oversampling)
{
	uint8_t i;
	for(i=0; i<Oversampling; i++)
	{
		if(val) bits[pos>>3] |= (0x1<<(pos&0x07));
		pos++;
	}
	return pos;
}

static inline void wave_frame_set(const uint8_t val)
{
	if(val) LL_GPIO_SetOutputPin(ECTx_GPIO_Port, ECTx_Pin);
	else LL_GPIO_ResetOutputPin(ECTx_GPIO_Port, ECTx_Pin);
}
// 过采样
void wave_frame_uart_Oversampling(void)
{
	// 过采样发送
	if((0==_wave_frame_uart._txl) || (_wave_frame_uart._txi>=_wave_frame_uart._txl))
	{
		if((_wave_frame_uart.txl>0) && (_wave_frame_uart.txi<_wave_frame_uart.txl))
		{
			// 取一个字节数据,将其展开成 bit 流
			uint8_t _byte = _wave_frame_uart.tx[_wave_frame_uart.txi++];  
			uint8_t index;
			uint8_t bit;
			index = 0;
			memset(_wave_frame_uart.bits, 0, sizeof(_wave_frame_uart.bits));
			index = wave_frame_bit(_wave_frame_uart.bits, index, 1, 3);
			index = wave_frame_bit(_wave_frame_uart.bits, index, 0, 5);     // start
			index = wave_frame_bit(_wave_frame_uart.bits, index, (_byte>>0)&0x01, 5);     // bit0
			index = wave_frame_bit(_wave_frame_uart.bits, index, (_byte>>1)&0x01, 5);     // bit1
			index = wave_frame_bit(_wave_frame_uart.bits, index, (_byte>>2)&0x01, 5);     // bit2
			index = wave_frame_bit(_wave_frame_uart.bits, index, (_byte>>3)&0x01, 5);     // bit3
			index = wave_frame_bit(_wave_frame_uart.bits, index, (_byte>>4)&0x01, 5);     // bit4
			index = wave_frame_bit(_wave_frame_uart.bits, index, (_byte>>5)&0x01, 5);     // bit5
			index = wave_frame_bit(_wave_frame_uart.bits, index, (_byte>>6)&0x01, 5);     // bit6
			index = wave_frame_bit(_wave_frame_uart.bits, index, (_byte>>7)&0x01, 5);     // bit7
			index = wave_frame_bit(_wave_frame_uart.bits, index, 1, 2);    // stop
			
			_wave_frame_uart._txl = index;
			_wave_frame_uart._txi = 0;
			// 发送一个 bit
			bit = (_wave_frame_uart.bits[_wave_frame_uart._txi/8]>>(_wave_frame_uart._txi&0x07)) & 0x01;
			wave_frame_set(bit);
			_wave_frame_uart._txi++;
		}
		else
		{
			_wave_frame_uart.txi = 0;
			_wave_frame_uart.txl = 0;
		}
		return ;
	}
	if(_wave_frame_uart._txi<_wave_frame_uart._txl)
	{
		uint8_t bit;
		// 发送一个 bit
		bit = (_wave_frame_uart.bits[_wave_frame_uart._txi/8]>>(_wave_frame_uart._txi&0x07)) & 0x01;
		wave_frame_set(bit);
		_wave_frame_uart._txi++;
	}
	// 过采样接收
}

uint8_t _wave_frame_uart_send(const uint8_t data[], const uint8_t _len)
{
	// 正在发送
	if(0!=_wave_frame_uart.txl) return 0;
	// 数据太长
	if(_len>sizeof(_wave_frame_uart.tx)) return 0;
	memcpy(_wave_frame_uart.tx, data, _len);
	_wave_frame_uart.txl = _len;
	return _len;
}
// 将需要发送的长数据拆分
uint16_t wave_frame_uart_send(const uint8_t data[], const uint16_t _len)
{
	uint16_t len;
	uint16_t txl;
	uint8_t _size = sizeof(_wave_frame_uart.tx);
	len=0;
	txl=0;
	while(len<_len)
	{
		txl = _len - len;
		if(txl>_size) txl=_size;
		while(0==_wave_frame_uart_send(&data[len], txl));
		len += txl;
	}
	// 正在发送
	if(0!=_wave_frame_uart.txl) return 0;
	// 数据太长
	if(len>sizeof(_wave_frame_uart.tx)) return 0;
	memcpy(_wave_frame_uart.tx, data, len);
	_wave_frame_uart.txl = len;
	return len;
}

/* TIM2 init function */
void _wave_frame_uart_Init(const uint16_t baud)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* TIM2 interrupt Init */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  //TIM_InitStruct.Autoreload = 839;
  TIM_InitStruct.Autoreload = 84000000/baud/5;   // 使用 5倍过采样率
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);
  LL_TIM_DisableMasterSlaveMode(TIM2);

}

