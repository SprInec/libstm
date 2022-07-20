/**
 ******************************************************************************
 * @file		:bsp_tim.c
 * @brief		:The board support package for tim.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.29
 ******************************************************************************
 */
 
#include "bsp_tim.h"
 
bsptimTypedef bsptim = {
	.freq = 		0,
};

 
/**
	* @brief  定时器中断回调/频率采集函数
	* @param
	*/
#if 0
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &TIM_IT_HANDLE)
	{
			bsptim.freq = __HAL_TIM_GetCounter(&TIM_IC_HANDLE);
			TIM_IC_ID->CNT = 0;
	}
}
#endif