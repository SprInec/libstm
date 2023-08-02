/**
 * @file bsp_fft.h
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023-04-04
 * @update 2023.07.24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_FFT_H__
#define __BSP_FFT_H__

#include "bsp_config.h"

#if __CMSIS_DSP
#include "arm_math_types.h"

#define BSP_FFT_SAMPLE_NUMBER 4096 /* 采样点数 */
#define BSP_FFT_SAMPLE_FREQUENCY (168000000/(5 + 10)) /* 采样频率 */
#define BSP_FFT_SAMPLE_STEP ((float)(BSP_FFT_SAMPLE_FREQUENCY / BSP_FFT_SAMPLE_NUMBER)) /* 采样步长 */
#define BSP_FFT_SAMPLE_STEP_READ 205.7613f

/**
 * @brief bsp fft struct
 *
 * @param input[] : input buffer
 * @param output[] : output buffer
 * @param amplitude : amplitude
 * @param frequency : frequency
 * @param subscript : subscript
 */
typedef struct{
		uint16_t subscript;
    float32_t input[BSP_FFT_SAMPLE_NUMBER * 2];
    float32_t output[BSP_FFT_SAMPLE_NUMBER];
    float32_t ampltitude;
    float32_t frequency;
}BSP_FFTTypedef;

extern uint16_t sample_data[BSP_FFT_SAMPLE_NUMBER];
extern BSP_FFTTypedef bsp_fft_handle;

void bsp_arm_fft(BSP_FFTTypedef *fft_handle, uint16_t *sample_data, uint16_t debug);
void BSP_FFT(BSP_FFTTypedef *fft_handle, uint16_t *sample_data, uint16_t debug);
#endif
#endif
