/**
 * @file bsp_timbase.h
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.11.30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __BSP_TIMBASE_H
#define __BSP_TIMBASE_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "bsp_config.h"

#define TIMBASE_TIM USE_TIM6

#if (TIMBASE_TIM == USE_TIM6)
#define FREERTOS_TIMBASE                  TIM6
#define FREERTOS_TIMBASE_Period           1000 - 1
#define FREERTOS_TIMBASE_Prescaler        71
#define FREERTOS_TIMBASE_IRQn             TIM6_DAC_IRQn
#define FREERTOS_TIMBASE_IRQHandler       TIM6_DAC_IRQHandler

#elif (TIMBASE_TIM == USE_TIM7)
#define FREERTOS_TIMBASE                  TIM7
#define FREERTOS_TIMBASE_Period           1000 - 1
#define FREERTOS_TIMBASE_Prescaler        71
#define FREERTOS_TIMBASE_IRQn             TIM7_IRQn
#define FREERTOS_TIMBASE_IRQHandler       TIM7_IRQHandler
#endif

void BSP_FREERTOS_TIMBASE_Init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* !__BSP_TIMBASE_H */
