/**
 * @file bsp_adf4351.h
 * @author Web
 * @brief 
 * @version 0.1
 * @date 2023.05.12
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _ADF4351_H_
#define _ADF4351_H_

#include "bsp_config.h"
#include "main.h"
#include "gpio.h"

/* 输出 */
#define ADF4351_CE(n) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, n)
#define ADF4351_LE(n) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, n)
#define ADF4351_OUTPUT_DATA(n) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, n)
#define ADF4351_CLK(n) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, n)

/* 读取 */
#define ADF4351_RF_OFF ((uint32_t)0XEC801C)

/* 输入源4分频后频率 */
#define FREQ_INSOURCE 25.0

void BSP_ADF4351_Init(void);
void ADF4351Init(void);
void ReadToADF4351(uint8_t count, uint8_t *buf);
void WriteToADF4351(uint8_t count, uint8_t *buf);
void WriteOneRegToADF4351(uint32_t Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre); //	(xx.x) M Hz??

#endif
