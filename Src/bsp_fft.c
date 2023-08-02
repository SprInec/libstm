/**
 * @file bsp_fft.c
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023-04-04
 * @update 2023.07.24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <math.h>
#include "bsp_fft.h"

#if __CMSIS_DSP
#include "arm_math.h"
#include "arm_const_structs.h"

uint16_t sample_data[BSP_FFT_SAMPLE_NUMBER];
BSP_FFTTypedef bsp_fft_handle = {
    .subscript = 0,
    .input = {0},
    .output = {0},
    .ampltitude = 0,
    .frequency = 0,
};

/**
 * @brief 快速傅里叶变换
 * @param fft_handle fft句柄
 * @param sample_data 数据
 * @param debug 是否打印
 */
void bsp_arm_fft(BSP_FFTTypedef *fft_handle, uint16_t *sample_data, uint16_t debug)
{
    /* 原始波形 */
    if (debug)
    {
        for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER; i++)
            bsprif1("sample_data: %.2f\n", sample_data[i] * 3.3 / 4096);
    }

    for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER; i++)
    {
        fft_handle->input[i * 2] = sample_data[i] / 4096.0 * 3.3 * 2;
        fft_handle->input[i * 2 + 1] = 0.0;
    }

    /* FFT 输入 */
    if (debug)
    {
        for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER * 2; i++)
            bsprif1("input: %.2f\n", fft_handle->input[i * 2 + 1]);
    }
    arm_cfft_f32(&arm_cfft_sR_f32_len4096, fft_handle->input, 0, 1);
    arm_cmplx_mag_f32(fft_handle->input, fft_handle->output, BSP_FFT_SAMPLE_NUMBER);

    /* 频谱图 */
    if (debug)
    {
        for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER / 2; i++)
            bsprif1("output: %.2f\n", fft_handle->output[i]);
    }

    arm_max_f32((fft_handle->output + 1), BSP_FFT_SAMPLE_NUMBER / 2, &fft_handle->ampltitude, (uint32_t *)&fft_handle->subscript);
    fft_handle->ampltitude = fft_handle->ampltitude * 2 / BSP_FFT_SAMPLE_NUMBER;
    fft_handle->frequency = (fft_handle->subscript * BSP_FFT_SAMPLE_STEP_READ);

    /* 波形属性 */
    if (debug)
    {
        bsprif1("place:%d\n", fft_handle->subscript);
        bsprif1("frequency:%.2f\n", fft_handle->frequency);
        bsprif1("ampltitude:%.2f\n", fft_handle->ampltitude);
    }
}

/* 谐波分量下标 */
uint16_t place[HARM_FREQ_NUM];
/* 谐波分量频率 */
float32_t frequency[HARM_FREQ_NUM];
/**
 * @brief 快速傅里叶变换
 * @param fft_handle fft句柄
 * @param sample_data 数据
 * @param debug 是否打印
 */
void BSP_FFT(BSP_FFTTypedef *fft_handle, uint16_t *sample_data, uint16_t debug)
{
    for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER; i++)
    {
        /* x10 增益 */
        fft_handle->input[i * 2] = sample_data[i] / 4096.0 * 3.3 * 10;
        fft_handle->input[i * 2 + 1] = 0.0;
    }

    arm_cfft_f32(&arm_cfft_sR_f32_len4096, fft_handle->input, 0, 1);
    arm_cmplx_mag_f32(fft_handle->input, fft_handle->output, BSP_FFT_SAMPLE_NUMBER);

    /* 频带处理 小于5的分量->0 */
    for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER / 2; i++)
    {
        if (fft_handle->output[i] < 10)
            fft_handle->output[i] = 0;
    }

    /* 频谱图-1 */
    if (debug)
    {
        for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER / 2; i++)
            bsprif1("output: %.2f\n", fft_handle->output[i]);
    }

    /* 频谱扫描 */
    for (uint8_t i = 0; i < HARM_FREQ_NUM; i++)
    {
        /* 寻找第 i 个谐波分量 */
        arm_max_f32((fft_handle->output + 1), BSP_FFT_SAMPLE_NUMBER / 2, &fft_handle->ampltitude, (uint32_t *)&fft_handle->subscript);
        fft_handle->ampltitude = fft_handle->ampltitude * 2 / BSP_FFT_SAMPLE_NUMBER;
        fft_handle->frequency = (fft_handle->subscript * BSP_FFT_SAMPLE_STEP_READ);
        /* 记录第 i 个谐波分量信息 */
        place[i] = fft_handle->subscript;
        frequency[i] = fft_handle->frequency;
        /* 陷波器 */
        for (int8_t j = -(NOTCH_NUM / 2); j < (NOTCH_NUM / 2); j++)
        {
            fft_handle->output[place[i] + 1 + j] = 0;
        }
    }

    /* 频谱图-2 */
    if (debug)
    {
        for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER / 2; i++)
            bsprif1("output: %.2f\n", fft_handle->output[i]);
    }

    /* 频率取整 */
    for (uint8_t i = 0; i < HARM_FREQ_NUM; i++)
    {
        float32_t temp = frequency[i] / 1000;
        if (temp < 58)
        {
            if (temp >= 18 && temp < 22){
                frequency[i] = 20000;
            }
            else if (temp >= 23 && temp < 27){
                frequency[i] = 25000;
            }
            else if (temp >= 28 && temp < 32){
                frequency[i] = 30000;
            }
            else if (temp >= 33 && temp < 37)
            {
                frequency[i] = 35000;
            }
            else if (temp >= 38 && temp < 42)
            {
                frequency[i] = 40000;
            }
            else if (temp >= 43 && temp < 47)
            {
                frequency[i] = 45000;
            }
            else if (temp >= 48 && temp <= 52)
            {
                frequency[i] = 50000;
            }
            else
            {
                frequency[i] = 55000;
            }
        }
        else
        {
            if (temp >= 58 && temp < 62)
            {
                frequency[i] = 60000;
            }
            else if (temp >= 63 && temp < 67)
            {
                frequency[i] = 65000;
            }
            else if (temp >= 68 && temp < 72)
            {
                frequency[i] = 70000;
            }
            else if (temp >= 73 && temp < 77)
            {
                frequency[i] = 75000;
            }
            else if (temp >= 78 && temp < 82)
            {
                frequency[i] = 80000;
            }
            else if (temp >= 83 && temp < 87)
            {
                frequency[i] = 85000;
            }
            else if (temp >= 88 && temp < 92)
            {
                frequency[i] = 90000;
            }
            else if (temp >= 93 && temp < 97)
            {
                frequency[i] = 95000;
            }
            else
            {
                frequency[i] = 100000;
            }
        }
    }

    /* 分析结果输出-1 */
    if (debug)
    {
        for (uint8_t i = 0; i < HARM_FREQ_NUM; i++)
        {
            bsprif1("place%d:%d\n", i, place[i]);
            bsprif1("frequency%d:%.2f\n", i, frequency[i]);
        }
    }

    /* 谐波分量中
        1,2 -> 双波主频
        3,4 -> 区分正弦与三角
    */
    wave1.freq = (uint16_t)frequency[0];
    wave2.freq = (uint16_t)frequency[1];
    if ((frequency[2] / 3 == wave1.freq) || frequency[3] / 3 == wave1.freq)
    {
        wave1.type = TRI;
    }
    else
    {
        wave1.type = SIN;
    }
    if ((frequency[2] / 3 == wave2.freq) || frequency[3] / 3 == wave2.freq)
    {
        wave1.type = TRI;
    }
    else
    {
        wave1.type = SIN;
    }

    /* 分析结果输出-2 */
    bsprif1("wave 1 freq: %d\n", wave1.freq);
    if (wave1.type == SIN)
        bsprif1("wave 1 type: SIN\n");
    else
        bsprif1("wave 1 type: TRI\n");

    bsprif1("wave 2 freq: %d\n", wave2.freq);
    if (wave2.type == SIN)
        bsprif1("wave 2 type: SIN\n");
    else
        bsprif1("wave 2 type: TRI\n");
}
#endif
