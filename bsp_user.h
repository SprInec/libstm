/**
 * @file bsp_user.h
 * @author ...
 * @brief User-defined function files
 * @version 0.1
 * @date ...
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_USER_H__
#define __BSP_USER_H__
#include "bsp_config.h"

#define ADC_SAMPLE_NUMBER 4096


/* 9959输出通道 */
#define OUTPUT_CHINNAL 0

/* ADC采集值 */
extern uint16_t adc_value[ADC_SAMPLE_NUMBER];
/* ADC采集完成标志 */
extern uint8_t adc_flag;

void BSP_Init(void);
void BSP_SysTemControl(void);

#endif /* __BSP_USER_H__ */
