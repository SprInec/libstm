/**
 * @file bsp_fft.c
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023-04-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <math.h>
#include "arm_math.h"
#include "arm_const_structs.h"
#include "bsp_fft.h"

uint16_t sample_data[BSP_FFT_SAMPLE_NUMBER];
fftbspTypedef bsp_fft_handle = {0, 0, 0, 0, 0};

void bsp_arm_fft(fftbspTypedef * fft_handle, uint16_t * sample_data)
{
#if 0
    for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER; i++)
        bsprif1("%.2f\n", sample_data[i]);
#endif
    for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER; i++)
    {
        fft_handle->input[i * 2] = sample_data[i] / 4096.0 * 3.3;
        fft_handle->input[i * 2 + 1] = 0.0;
    }
#if 1
    for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER * 2; i++)
        bsprif1("%.2f\n", fft_handle->input[i * 2 + 1]);
#endif
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_handle->input, 0, 1);
    arm_cmplx_mag_f32(fft_handle->input, fft_handle->output, BSP_FFT_SAMPLE_NUMBER);

#if 0
    for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER / 2; i++)
        bsprif1("%.2f\n", fft_handle->output[i]);
#endif

    arm_max_f32(fft_handle->output, BSP_FFT_SAMPLE_NUMBER / 2, &fft_handle->ampltitude, (uint32_t *)&fft_handle->subscript);
    fft_handle->ampltitude = fft_handle->ampltitude * 2 / BSP_FFT_SAMPLE_NUMBER;
    fft_handle->frequency = (fft_handle->subscript * BSP_FFT_SAMPLE_STEP);

#if 0
    bsprif1("frequency:%.2f\n", fft_handle->frequency);
    bsprif1("ampltitude:%.2f\n", fft_handle->ampltitude);
#endif
}
