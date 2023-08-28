/**
 * @file bsp_delay.h
 * @author July (Email: JulyCub@163.com)
 * @brief bsp delay function
 * @version 0.1
 * @date 2023.05.11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"
#include "bsp_sys.h"

#define BSP_SYSTICK_DELAY 0
#define BSP_TIMER_DELAY 1

#if BSP_TIMER_DELAY
#include "tim.h"
#define DELAYTIMER htim7
void delay_init(void);
#endif

void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#ifdef __cplusplus
}
#endif
#endif /* __CORE_DELAY_H */
