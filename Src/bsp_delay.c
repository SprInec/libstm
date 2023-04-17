/*
 *****************************************************************************
 * @file		:bsp_delay.c
 * @brief		:使用SysTick的普通计数模式对延迟进行管理.
 * @version		:1.1
 * @author		:merchant
 * @date		:2022.07.05	-v0
 * @update		:2023.02.27	-v1
 * @state		:Enable
 *****************************************************************************
 */


#include "bsp_delay.h"

#if BSP_SYSTICK_DELAY
void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->LOAD = 18 * nus;
	SysTick->VAL = 0X00;
	SysTick->CTRL = 0X01;
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && (!(temp & (1 << 16))));
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0X00;
}

void delay_ms(uint16_t nms)
{
	uint32_t temp;
	SysTick->LOAD = 500 * nms;
	SysTick->VAL = 0X00;
	SysTick->CTRL = 0X01;
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && (!(temp & (1 << 16))));
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0X00;
}

void delay_100ns(void)
{
	/* -- */
}*/
#endif

#if BSP_SYSTICK2_DELAY
void bsp_delayB_us(uint32_t nus)
{
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
	HAL_Delay(nus - 1);
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
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