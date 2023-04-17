/*
 ******************************************************************************
 * @file		:bsp_led.c
 * @brief		:The board support package for led.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.26
 ******************************************************************************
 */

#include "bsp_led.h"


void bsp_led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

