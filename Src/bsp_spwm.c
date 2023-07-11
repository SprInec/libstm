/**
 * @file bsp_spwm.c
 * @author July (Email: JulyCub@163.com)
 * @brief spwm driver
 * @version 0.1
 * @date 2023.07.03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bsp_spwm.h"
#include "gpio.h"
#include "tim.h"

TIM_HandleTypeDef spwm_tim;

/**
 * @brief SPWM initialization
 * @attention The SPWM use TIM10 defaultly
 *            TIM10_CH1 -> PF6
 */
void BSP_SPWM_Init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0};

    htim10.Instance = TIM10;
    htim10.Init.Prescaler = 0;
    htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim10.Init.Period = 65535;
    htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_PWM_Init(&htim10) != HAL_OK)
        Error_Handler();

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
        Error_Handler();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    __HAL_RCC_TIM10_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
}


