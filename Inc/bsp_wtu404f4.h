/**
 * @file bsp_wtu404f4.h
 * @author SprInec (JulyCub@163.com)
 * @brief WTU404F4-B004 高精度红外模块
 * @version 0.1
 * @date 2025.01.08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __BSP_WTU404F4_H
#define __BSP_WTU404F4_H
#if __cplusplus
extern "C"
{
#endif
#include "bsp_config.h"

#define WTU404F4_MODE_INT 0x01
#define WTU404F4_MODE_UART 0x02

/* 选择模式 */
#define WTU404F4_MODE WTU404F4_MODE_INT

#if WTU404F4_MODE == WTU404F4_MODE_INT
#define WTU404F4_INT_PORT GPIOC
#define WTU404F4_DAT_PORT GPIOD
#define WTU404F4_INT_PIN GPIO_PIN_6
#define WTU404F4_DAT_PIN GPIO_PIN_15

#define __BSP_RCC_WTU404F4_CLK_ENABLE() { \
	__HAL_RCC_GPIOC_CLK_ENABLE();    \
	__HAL_RCC_GPIOD_CLK_ENABLE();    \
}

void BSP_WTU404F4_InfraredLearningMode(void);

#else
#include "usart.h"

#define WTU404F4_UART huart3

// 定义红外感应模块的命令头
#define WTU404F4_HEADER 0x00
#define WTU404F4_RECLEN 9

#define __BSP_WTU404F4_SEND(_code_) HAL_UART_Transmit(&WTU404F4_UART, _code_, sizeof(_code_), 0xFF)
#define __BSP_WTU404F4_RECEIVE(_data_) HAL_UART_Receive(&WTU404F4_UART, _data_, WTU404F4_RECLEN, 0xFFFF)

uint8_t BSP_WTU404F4_Init(void);
uint8_t BSP_WTU404F4_InfraredLearningMode(void);
uint8_t BSP_WTU404F4_InfraredResponseSpeed(uint8_t speed);
uint8_t BSP_WTU404F4_InfraredEnergyIntensity(uint8_t intensity);
uint8_t BSP_WTU404F4_InfraredINT(uint8_t level);
uint8_t BSP_WTU404F4_InfraredDataReport(uint8_t enable);
uint8_t BSP_WTU404F4_AckData(uint8_t oder);
uint8_t BSP_WTU404F4_InfraredDetection(uint8_t data[]);
#endif /* WTU404F4_MODE */

#if __cplusplus
}
#endif
#endif /* __BSP_WTU404F4_H */