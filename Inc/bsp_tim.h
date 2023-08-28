/**
 ******************************************************************************
 * @file		:bsp_tim.h
 * @brief		:The board support package for tim.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.29
 ******************************************************************************
 */
 
#ifndef __BSP_TIM_H__
#define __BSP_TIM_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"


#define TIM_IT_HANDLE			htim4
#define TIM_IC_HANDLE			htim2
#define TIM_IC_ID					TIM2

typedef struct tim{
	uint32_t		freq;
}bsptimTypedef;



extern bsptimTypedef bsptim;

#ifdef __cplusplus
}
#endif
#endif
