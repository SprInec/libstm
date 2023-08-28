/**
 * @file bsp_gp2y.c
 * @author fire
 * @brief PM2.5/粉尘检测
 * @version 0.1
 * @date 2023.08.22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_GP2Y_H__
#define __BSP_GP2Y_H__

#include "bsp_config.h"

#define MAX_NUM 10
#define RATIO 0.2           /*灰尘颗粒浓度与电压值比例系数*/
#define NO_DUST_VOLTAGE 400 /*灰尘浓度为0时的输出电压值*/

// 引脚定义
#define GP2Y_ADC_PIN GPIO_PIN_10
#define GP2Y_ADC_GPIO_PORT GPIOF
#define GP2Y_ADC_GPIO_CLK_ENABLE() __GPIOF_CLK_ENABLE()

// ADC 序号宏定义
#define GP2Y_ADC ADC3
#define GP2Y_ADC_CLK_ENABLE() __ADC3_CLK_ENABLE()
#define GP2Y_ADC_CHANNEL ADC_CHANNEL_6
// DMA时钟使能
#define GP2Y_ADC_DMA_CLK_ENABLE() __HAL_RCC_DMA1_CLK_ENABLE();
#define GP2Y_ADC_DMA_Base DMA1_Stream1
#define GP2Y_ADC_DMA_Request DMA_REQUEST_ADC3
// DMA中断服务函数
#define GP2Y_ADC_DMA_IRQHandler DMA1_Stream1_IRQHandler

#define GP2Y_ADC12_IRQ ADC_IRQn

// ILED GPIO宏定义
#define GP2Y_ILED_PIN GPIO_PIN_12
#define GP2Y_ILED_PORT GPIOA
#define GP2Y_ILED_CLK_ENABLE() __GPIOA_CLK_ENABLE()

#define ILED_HIGH (GPIO_PinState)1
#define ILED_LOW (GPIO_PinState)0
#define GP2Y_ILED_SET(state) HAL_GPIO_WritePin(GP2Y_ILED_PORT, GP2Y_ILED_PIN, state)

void BSP_GP2Y_Init(void);
double GP2Y_GetDens(void);
#endif /* !__BSP_GP2Y_H__ */
