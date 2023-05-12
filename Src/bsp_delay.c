/**
 * @file bsp_delay.c
 * @author July (Email: JulyCub@163.com)
 * @brief bsp delay function
 * @version 0.1
 * @date 2023.05.11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bsp_delay.h"

#if BSP_SYSTICK_DELAY
void delay_us(uint32_t nus)
{
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
	HAL_Delay(nus - 1);
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}

void delay_ms(uint16_t nms)
{
	HAL_Delay(nms);
}
#endif

#if BSP_TIMER_DELAY
void delay_us(uint32_t nus)
{
	uint16_t differ = 0xFFFF - nus - 5;

	__HAL_TIM_SetCounter(&DELAYTIMER, differ);
	HAL_TIM_Base_Start(&DELAYTIMER);
	while (differ < 0xFFFF - 5)
	{
		differ = __HAL_TIM_GetCounter(&DELAYTIMER);
	};
	HAL_TIM_Base_Stop(&DELAYTIMER);
}

void delay_ms(uint16_t nms)
{
	uint32_t timer;
	for (timer = 0; timer < nms; timer++)
		delay_us(1000);
}
#endif