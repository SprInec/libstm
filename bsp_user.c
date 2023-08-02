/**
 * @file bsp_user.c
 * @author ...
 * @brief User-defined function files
 * @version 0.1
 * @date ...
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_user.h"
#include "adc.h"
#include "tim.h"

/* ADC采集值 */
uint16_t adc_value[ADC_SAMPLE_NUMBER] = {0};
/* ADC采集完成标志 */
uint8_t adc_flag = 0;

/* 双波输出 */
OUT_WAVE wave1 = {
    .freq = 0,
    .amplitude = 0,
    .phase = 0,
    .type = SIN
};

OUT_WAVE wave2 = {
    .freq = 0,
    .amplitude = 0,
    .phase = 0,
    .type = SIN
};

void BSP_Init(void)
{
    BSP_LED_Init();
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&adc_value, ADC_SAMPLE_NUMBER);
    HAL_TIM_Base_Start(&htim2);

    AD9959_Init();
    AD9959_WriteAmpl(OUTPUT_CHINNAL, 1023);
    AD9959_WriteFreq(OUTPUT_CHINNAL, 10000);

    BSP_FFT(&bsp_fft_handle, adc_value, 0);
}

void BSP_SysTemControl(void)
{
    __BSP_LED2_Ficker(100);
    // if (adc_flag == 1)
    // {
    //     BSP_FFT(&bsp_fft_handle, adc_value, 1);
    //     HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&adc_value, 1024);
    //     adc_flag = 1;
    // }
}


/* CODE */
