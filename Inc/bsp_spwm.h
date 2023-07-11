/**
 * @file bsp_spwm.h
 * @author July (Email: JulyCub@163.com)
 * @brief spwm driver
 * @version 0.1
 * @date 2023.07.03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_SPWM_H__
#define __BSP_SPWM_H__

#include "bsp_config.h"

#define BSP_SPWM_PORT GPIOA
#define BSP_SPWM_PIN GPIO_PIN_2

typedef struct SPWM
{
    uint32_t phase;
    uint32_t M;
    uint32_t temp;
    uint32_t max;
    uint32_t real;
} BSP_SPWM_TypeDed;

extern TIM_HandleTypeDef spwm_tim;

void BSP_SPWM_Init(void);

#endif /* !__BSP_SPWM_H__ */
