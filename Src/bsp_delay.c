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

#if __RTOS_RTTHREAD_ENABLED

void delay_us(rt_uint32_t nus)
{
	rt_uint32_t ticks;
	rt_uint32_t told, tnow, tcnt = 0;
	rt_uint32_t reload = SysTick->LOAD;

	/* 获得延时经过的 tick 数 */
	ticks = nus * reload / (1000000 / RT_TICK_PER_SECOND);
	/* 获得当前时间 */
	told = SysTick->VAL;
	while (1)
	{
		/* 循环获得当前时间，直到达到指定的时间后退出循环 */
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
			{
				tcnt += told - tnow;
			}
			else
			{
				tcnt += reload - tnow + told;
			}
			told = tnow;
			if (tcnt >= ticks)
			{
				break;
			}
		}
	}
}

void delay_ms(rt_uint16_t nms)
{
	rt_thread_mdelay(nms);
}

#elif __RTOS_FREERTOS_ENABLED
void delay_ms(uint16_t nms)
{	
	vTaskDelay(pdMS_TO_TICKS(nms));
}
#else
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
#endif