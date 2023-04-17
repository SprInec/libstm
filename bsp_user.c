/**
 * @file bsp_user.c
 * @author July (Email: JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date xxx
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "bsp_user.h"
#include "bsp_statemac.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	BSP_KEY_StateTransition();
		
}