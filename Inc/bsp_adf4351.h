/**
 * @file bsp_adf4351.h
 * @author July (Email: JulyCub@163.com)
 * @brief ADF4351 DRIVER
 * @version 0.1
 * @date 2023.07.13
 * @attention ADF4351正弦波点频输出，范围35M-4400M，步进0.1M
 * @pinset
 * 			LE   ->  PG11
 *			CLK  ->  PG12
 *			CE   ->  PG13
 * 			DATA ->  PG14
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_ADF4351_H__
#define __BSP_ADF4351_H__

#include "bsp_config.h"
#include "main.h"
#include "gpio.h"

/* 输出 */
#define ADF4351_CE(n) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, n)
#define ADF4351_LE(n) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, n)
#define ADF4351_OUTPUT_DATA(n) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, n)
#define ADF4351_CLK(n) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, n)

#define ADF4351_R0 ((uint32_t)0X2C8018)
#define ADF4351_R1 ((uint32_t)0X8029)
#define ADF4351_R2 ((uint32_t)0X10E42)
#define ADF4351_R3 ((uint32_t)0X4B3)
#define ADF4351_R4 ((uint32_t)0XEC803C)
#define ADF4351_R5 ((uint32_t)0X580005)

#define ADF4351_R1_Base ((uint32_t)0X8001)
#define ADF4351_R4_Base ((uint32_t)0X8C803C)
#define ADF4351_R4_ON ((uint32_t)0X8C803C)
#define ADF4351_R4_OFF ((uint32_t)0X8C883C)

#define ADF4351_PD_ON ((uint32_t)0X10E42)
#define ADF4351_PD_OFF ((uint32_t)0X10E02)

#define ADF4351_RF_OFF ((uint32_t)0XEC801C)

/* 输入源4分频后频率 */
#define FREQ_INSOURCE 25.0

void BSP_ADF4351_Init(void);
void ADF4351Init(void);
void ReadToADF4351(uint8_t count, uint8_t *buf);
void WriteToADF4351(uint8_t count, uint8_t *buf);
void WriteOneRegToADF4351(uint32_t Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre);

#endif /* __BSP_ADF4351_H__ */
