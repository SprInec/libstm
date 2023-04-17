/**
 * @file bsp_fft.h
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023-04-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_FFT_H__
#define __BSP_FFT_H__

#include "bsp_config.h"
#include "arm_math_types.h"

#define BSP_FFT_SAMPLE_NUMBER 1024
#define BSP_FFT_SAMPLE_FREQUENCY (84000000/(84 + 100))
#define BSP_FFT_SAMPLE_STEP ((float)(BSP_FFT_SAMPLE_FREQUENCY / BSP_FFT_SAMPLE_NUMBER))

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
}fftbspTypedef;

extern uint16_t sample_data[BSP_FFT_SAMPLE_NUMBER];
extern fftbspTypedef bsp_fft_handle;

void bsp_arm_fft(fftbspTypedef * fft_handle, uint16_t * sample_data);

#endif
