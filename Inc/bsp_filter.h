/**
 * @file bsp_filter.c
 * @author July (Email: JulyCub@163.com)
 * @brief The board support package for filter.
 * @version 0.1
 * @date 2022.07.02
 * @update 2023.07.24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_FILTER_H__
#define __BSP_FILTER_H__

#include "bsp_config.h"

#if __CMSIS_DSP
#include "arm_math.h"

/* FIR */
#define FIR_LENGTH_SAMPLES 4096 /* 采样点数 */
#define FIR_BLOCK_SIZE 1        /* 调用一次arm_fir_f32处理的采样点个数 */
#define FIR_FACTOR_NUM 32       /* 滤波器系数个数 */

/* LMS */
#define LMS_LENGTH_SAMPLES 4096 /* 采样点数 */
#define LMS_BLOCK_SIZE 1        /* 调用一次arm_lms_f32处理的采样点个数 */

void bsp_arm_fir_f32(float32_t *data_buff,
                     float32_t *out_buff,
                     float32_t *cache_buff,
                     float32_t *factor,
                     uint32_t sample_num,
                     uint32_t block_size,
                     uint32_t num_taps,
                     uint8_t debug);

void bsp_arm_lms_f32(float32_t *data_buff,
                     float32_t *out_buff,
                     float32_t *ref_buff,
                     float32_t *err_buff,
                     float32_t *cache_buff,
                     float32_t *factor,
                     float32_t mu_num,
                     uint32_t sample_num,
                     uint32_t block_size,
                     uint32_t num_taps,
                     uint8_t debug);

#endif

    /* 平滑滤波算法 */
    float Onepointfilter(int16_t data, uint16_t fnum);
void linearSmooth3(double in[], double out[], int N);
void linearSmooth5(double in[], double out[], int N);
void linearSmooth7(double in[], double out[], int N);
void quadraticSmooth5(double in[], double out[], int N);
void quadraticSmooth7(double in[], double out[], int N);
void cubicSmooth5(double in[], double out[], int N);
void cubicSmooth7(double in[], double out[], int N);

#endif
