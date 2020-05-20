/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_UART_H
#define __HAL_UART_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "lib/cache_queue.h"

/* USER CODE BEGIN Prototypes */
__STATIC_INLINE int usart_send(USART_TypeDef *USARTx, const uint8_t data[], const uint32_t _size)
{
	uint8_t Value;
	uint32_t len;
	for(len=0; len<_size; len++)
	{
#if (USE_TIMEOUT == 1)
		Timeout = USART_SEND_TIMEOUT_TXE_MS;
#endif /* USE_TIMEOUT */

		/* Wait for TXE flag to be raised */
		while (!LL_USART_IsActiveFlag_TXE(USARTx))
		{
#if (USE_TIMEOUT == 1)
			/* Check Systick counter flag to decrement the time-out value */
			if (LL_SYSTICK_IsActiveCounterFlag())
			{
				if(Timeout-- == 0)
				{
					/* Time-out occurred. Set LED to blinking mode */
					LED_Blinking(LED_BLINK_SLOW);
				}
			}
#endif /* USE_TIMEOUT */
		}

		/* If last char to be sent, clear TC flag */
		/*if (ubSend == (sizeof(aStringToSend) - 1))
		{
		  LL_USART_ClearFlag_TC(USARTx);
		}*/

		/* Write character in Transmit Data register.
		   TXE flag is cleared by writing data in DR register */
		Value = data[len];
		LL_USART_TransmitData8(USARTx, Value);
	}
	LL_USART_ClearFlag_TC(USARTx);
	return _size;
}

extern cache_queue Debug_RX_cache;
extern cache_queue CCM_RX_cache;
extern cache_queue UART2_RX_cache;
extern cache_queue max485_cache;

static __inline void add_queue_debug(const uint8_t _byte)
{
	//__asm("CPSID  I");
	//cache_queue_write(&UART5_RX_cache, _byte);
	// 宏展开
	//macro_queue_write(_byte,Debug_RX_cache);
	// 内联函数
	cache_queue_write(&Debug_RX_cache, _byte);
	//__asm("CPSIE  I");
}
static __inline void add_queue_CMM(const uint8_t _byte)
{
	//__asm("CPSID  I");
	macro_queue_write(_byte,CCM_RX_cache);
	//__asm("CPSIE  I");
}
static __inline void add_queue_UART2(const uint8_t _byte)
{
	//__asm("CPSID  I");
	macro_queue_write(_byte,UART2_RX_cache);
	//__asm("CPSIE  I");
}
static __inline void add_queue_max485(const uint8_t _byte)
{
	//__asm("CPSID  I");
	macro_queue_write(_byte,max485_cache);
	//__asm("CPSIE  I");
}

/*_____________________________________________________________ 调试接口 ________________________________________________________________*/
extern int qDebug(const char *__format, ...);
extern int app_debug(const char *__format, ...);
extern int debug_send(const uint8_t data[], const uint32_t _size);
extern int debug_read(uint8_t buf[], const uint32_t _size);
/*_____________________________________________________________ 加密芯片接口 ________________________________________________________________*/
extern int ccm_send(const uint8_t data[], const uint32_t _size);
extern int CCM_read(uint8_t buf[], const uint32_t _size);
/*_____________________________________________________________ 485通信 ________________________________________________________________*/
extern int max485_send(const uint8_t data[], const uint32_t _size);
extern int max485_read(uint8_t buf[], const uint32_t _size);
extern void max485_init(void);
extern int max485_debug(const char *__format, ...);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ __HAL_UART_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
