/*
 ******************************************************************************
 * @file		:bsp_delay.h
 * @brief		:Õÿ’π—” ±
 * @version	    :1.0
 * @author	    :merchant
 * @date		:2022.07.05 -v0
 * @update      :2023.02.27 -v1
 * @state       :Enable
 ******************************************************************************
 */

#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__

#include "bsp_config.h"
#include "bsp_sys.h"
#include "tim.h"

#define BSP_SYSTICK_DELAY 0
#define BSP_SYSTICK2_DELAY 0
#define BSP_TIMER_DELAY 1

#if BSP_TIMER_DELAY
#define DELAYTIMER htim7
#endif

void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#if BSP_SYSTICK_DELAY
void delay_100ns(void);
#endif

#endif /* __CORE_DELAY_H */
