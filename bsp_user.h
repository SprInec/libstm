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
#define HARM_FREQ_NUM 4 /* 谐波个数 */
#define NOTCH_NUM 20    /* 陷波点数 */

/* 相加类型 */
typedef enum{
    SIN = 0,
    TRI = 1
} WAVE_TYPE;

typedef struct output_wave{
    float freq;
    uint16_t amplitude;
    uint8_t phase;
    WAVE_TYPE type;
} OUT_WAVE;

/* 9959输出通道 */
#define OUTPUT_CHINNAL 0
/* ADC采集值 */
extern uint16_t adc_value[ADC_SAMPLE_NUMBER];
/* ADC采集完成标志 */
extern uint8_t adc_flag;
extern OUT_WAVE wave1;
extern OUT_WAVE wave2;

void BSP_Init(void);
void BSP_SysTemControl(void);

#endif /* __BSP_USER_H__ */
