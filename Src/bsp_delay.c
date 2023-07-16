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
volatile uint32_t delay_counter = 0;
void delay_init(void)
{
	HAL_TIM_Base_Start(&DELAYTIMER);
}

void delay_us(uint32_t nus)
{
	volatile uint32_t t0, t1;
	t0 = delay_counter * 65535 + __HAL_TIM_GET_COUNTER(&DELAYTIMER);
	while(1)
	{
		t1 = delay_counter * 65535 + __HAL_TIM_GET_COUNTER(&DELAYTIMER);
		if (t1 - t0 >= nus){
			break;
		}
	}
}

void delay_ms(uint16_t nms)
{
	uint32_t timer;
	for (timer = 0; timer < nms; timer++){
		delay_us(1000);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == DELAYTIMER.Instance)
	{
		delay_counter++;
	}
}
#endif