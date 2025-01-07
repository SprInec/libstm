/**
 * @file bsp_timbase.c
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.11.29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "bsp_timbase.h"

volatile uint32_t CPU_RunTime = 0UL;
TIM_HandleTypeDef FREERTOS_TIMBASE_Handle;

static void FREERTOS_TIMBASE_Config(void)
{
    FREERTOS_TIMBASE_Handle.Instance = FREERTOS_TIMBASE;
    FREERTOS_TIMBASE_Handle.Init.Period = FREERTOS_TIMBASE_Period;
    FREERTOS_TIMBASE_Handle.Init.Prescaler = FREERTOS_TIMBASE_Prescaler;
    FREERTOS_TIMBASE_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    FREERTOS_TIMBASE_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&FREERTOS_TIMBASE_Handle) != HAL_OK)
    {
        Error_Handler();
    }

#if (TIMBASE_TIM == USE_TIM6)
    __HAL_RCC_TIM6_CLK_ENABLE();
#elif (TIMBASE_TIM == USE_TIM7)
    __HAL_RCC_TIM7_CLK_ENABLE();
#endif
    HAL_NVIC_SetPriority(FREERTOS_TIMBASE_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(FREERTOS_TIMBASE_IRQn);
}

void BSP_FREERTOS_TIMBASE_Init(void)
{
    FREERTOS_TIMBASE_Config();
    HAL_TIM_Base_Start_IT(&FREERTOS_TIMBASE_Handle);
}

void FREERTOS_TIMBASE_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&FREERTOS_TIMBASE_Handle);
    CPU_RunTime++;
}
