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
#ifndef __PERIPHS_GPIO_H
#define __PERIPHS_GPIO_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "lib/cache_queue.h"

// GPIO output
struct notify_led
{
	const uint32_t Periphs;
	GPIO_TypeDef *const GPIOx;
	const uint32_t PinMask;
	//__IO uint32_t* const CCRx;  // PWM模式使用此字段
};
// PWM output
struct notify_pwm
{
	// 定时器
	TIM_TypeDef *const TIMx;
	// PWM输出寄存器
	__IO uint32_t* const CCRx;  
};

static __inline void _led_Init(const struct notify_led* const led)
{

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	LL_APB2_GRP1_EnableClock(led->Periphs);
	/**/
	LL_GPIO_SetOutputPin(led->GPIOx, led->PinMask);
	/**/
	GPIO_InitStruct.Pin = led->PinMask;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(led->GPIOx, &GPIO_InitStruct);
}

static __inline void __notify_led_hight(const struct notify_led* const led, const int hight)
{
	if(hight) LL_GPIO_SetOutputPin(led->GPIOx, led->PinMask);
	else   LL_GPIO_ResetOutputPin(led->GPIOx, led->PinMask);
}

#ifdef __cplusplus
}
#endif
#endif /*__ __PERIPHS_GPIO_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
