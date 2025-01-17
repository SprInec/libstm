/**
 * @file bsp_waterpump.c
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2025.01.08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "bsp_tmi8260.h"
#include "gpio.h"

void BSP_TMI8260_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __BSP_RCC_TMI8260_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = TMI8260_IN1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TMI8260_IN1_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TMI8260_IN2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TMI8260_IN2_PORT, &GPIO_InitStruct);
}

/**
 * @brief 正转
 * @note IN1 - 1
 *       IN2 - 0
 */
void BSP_TMI8260_Forward(void)
{
    HAL_GPIO_WritePin(TMI8260_IN1_PORT, TMI8260_IN1_PIN, TMI8260_IN_1);
    HAL_GPIO_WritePin(TMI8260_IN2_PORT, TMI8260_IN2_PIN, TMI8260_IN_0);
}

/**
 * @brief 反转
 * @note IN1 - 0
 *       IN2 - 1
 */
void BSP_TMI8260_Reverse(void)
{
    HAL_GPIO_WritePin(TMI8260_IN1_PORT, TMI8260_IN1_PIN, TMI8260_IN_0);
    HAL_GPIO_WritePin(TMI8260_IN2_PORT, TMI8260_IN2_PIN, TMI8260_IN_1);
}

/**
 * @brief 刹车
 * @note IN1 - 0
 *       IN2 - 0
 */
void BSP_TMI8260_Brake(void)
{
    HAL_GPIO_WritePin(TMI8260_IN1_PORT, TMI8260_IN1_PIN, TMI8260_IN_0);
    HAL_GPIO_WritePin(TMI8260_IN2_PORT, TMI8260_IN2_PIN, TMI8260_IN_0);
}

/**
 * @brief 自由停止
 * @note IN1 - 1
 *       IN2 - 1
 */
void BSP_TMI8260_FreeStop(void)
{
    HAL_GPIO_WritePin(TMI8260_IN1_PORT, TMI8260_IN1_PIN, TMI8260_IN_1);
    HAL_GPIO_WritePin(TMI8260_IN2_PORT, TMI8260_IN2_PIN, TMI8260_IN_1);
}
