/**
 * @file bsp_tmi8260.h
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2025.01.08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __BSP_TMI8260_H
#define __BSP_TMI8260_H
#ifdef __cplusplus
extern "C" {
#endif
#include "bsp_config.h"

#define TMI8260_IN1_PORT GPIOE
#define TMI8260_IN2_PORT GPIOA

#define __BSP_RCC_TMI8260_CLK_ENABLE() { \
    __HAL_RCC_GPIOE_CLK_ENABLE();        \
    __HAL_RCC_GPIOA_CLK_ENABLE();        \
}

#define TMI8260_IN1_PIN GPIO_PIN_9
#define TMI8260_IN2_PIN GPIO_PIN_7

#define TMI8260_IN_0 GPIO_PIN_RESET
#define TMI8260_IN_1 GPIO_PIN_SET

void BSP_TMI8260_Init(void);
void BSP_TMI8260_Forward(void);
void BSP_TMI8260_Reverse(void);
void BSP_TMI8260_Brake(void);
void BSP_TMI8260_FreeStop(void);

#ifdef __cplusplus 
}
#endif
#endif /* __BSP_TMI8260_H */