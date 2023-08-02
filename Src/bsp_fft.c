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
/* 谐波分量幅值 */
float32_t ampl[HARM_FREQ_NUM];
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
    // if (debug)
    {
        for (uint16_t i = 0; i < BSP_FFT_SAMPLE_NUMBER / 2; i++)
            bsprif1("output: %.2f\n", fft_handle->output[i]);
    }

    /* 频谱扫描 */
    for (uint8_t i = 0; i < HARM_FREQ_NUM; i++)
    {
        /* 寻找第 i 个谐波分量 */
        arm_max_f32((fft_handle->output + 1), BSP_FFT_SAMPLE_NUMBER / 2, &fft_handle->ampltitude, (uint32_t *)&fft_handle->subscript);
        fft_handle->ampltitude = fft_handle->ampltitude / BSP_FFT_SAMPLE_NUMBER / 1.66;
        fft_handle->frequency = (fft_handle->subscript * BSP_FFT_SAMPLE_STEP_READ);
        /* 记录第 i 个谐波分量信息 */
        place[i] = fft_handle->subscript;
        frequency[i] = fft_handle->frequency;
        ampl[i] = fft_handle->ampltitude;
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

    /* 分析结果输出-1 */
    if (debug)
    {
        for (uint8_t i = 0; i < HARM_FREQ_NUM; i++)
        {
            bsprif1("place%d:%d \n", i, place[i]);
            bsprif1("frequency%d:%.2f \n", i, frequency[i]);
            bsprif1("ampl%d, %.2f\n ", i, ampl[i]);
        }
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
        else if (temp < 102)
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
        else if (temp < 147)
        {
            if (temp >= 103 && temp < 107)
            {
                frequency[i] = 105000;
            }
            else if (temp >= 108 && temp < 112)
            {
                frequency[i] = 110000;
            }
            else if (temp >= 113 && temp < 117)
            {
                frequency[i] = 115000;
            }
            else if (temp >= 118 && temp < 122)
            {
                frequency[i] = 120000;
            }
            else if (temp >= 123 && temp < 127)
            {
                frequency[i] = 125000;
            }
            else if (temp >= 128 && temp < 132)
            {
                frequency[i] = 130000;
            }
            else if (temp >= 133 && temp < 137)
            {
                frequency[i] = 135000;
            }
            else if (temp >= 138 && temp < 142)
            {
                frequency[i] = 140000;
            }
            else
            {
                frequency[i] = 145000;
            }
        }
        else if (temp < 197)
        {
            if (temp >= 148 && temp < 152)
            {
                frequency[i] = 150000;
            }
            else if (temp >= 153 && temp < 157)
            {
                frequency[i] = 155000;
            }
            else if (temp >= 158 && temp < 162)
            {
                frequency[i] = 160000;
            }
            else if (temp >= 163 && temp < 167)
            {
                frequency[i] = 165000;
            }
            else if (temp >= 168 && temp < 172)
            {
                frequency[i] = 170000;
            }
            else if (temp >=173 && temp < 177)
            {
                frequency[i] = 175000;
            }
            else if (temp >= 178 && temp < 182)
            {
                frequency[i] = 180000;
            }
            else if (temp >= 183 && temp < 187)
            {
                frequency[i] = 185000;
            }
            else
            {
                frequency[i] = 190000;
            }
        }
        else if (temp < 237)
        {
            if (temp >= 193 && temp < 197)
            {
                frequency[i] = 195000;
            }
            else if (temp >= 198 && temp < 202)
            {
                frequency[i] = 200000;
            }
            else if (temp >= 203 && temp < 207)
            {
                frequency[i] = 205000;
            }
            else if (temp >= 208 && temp < 212)
            {
                frequency[i] = 210000;
            }
            else if (temp >= 213 && temp < 217)
            {
                frequency[i] = 215000;
            }
            else if (temp >= 218 && temp < 222)
            {
                frequency[i] = 220000;
            }
            else if (temp >= 223 && temp < 227)
            {
                frequency[i] = 225000;
            }
            else if (temp >= 228 && temp < 232)
            {
                frequency[i] = 230000;
            }
            else
            {
                frequency[i] = 235000;
            }
        }
        else
        {
            if (temp >= 238 && temp < 242)
            {
                frequency[i] = 240000;
            }
            else if (temp >= 243 && temp < 247)
            {
                frequency[i] = 245000;
            }
            else if (temp >= 248 && temp < 252)
            {
                frequency[i] = 250000;
            }
            else if (temp >= 253 && temp < 257)
            {
                frequency[i] = 255000;
            }
            else if (temp >= 258 && temp < 262)
            {
                frequency[i] = 260000;
            }
            else if (temp >= 263 && temp < 267)
            {
                frequency[i] = 265000;
            }
            else if (temp >= 268 && temp < 272)
            {
                frequency[i] = 270000;
            }
            else if (temp >= 273 && temp < 277)
            {
                frequency[i] = 275000;
            }
            else if (temp >= 278 && temp < 282)
            {
                frequency[i] = 280000;
            }
            else if (temp >= 283 && temp < 287)
            {
                frequency[i] = 285000;
            }
            else if (temp >= 288 && temp < 292)
            {
                frequency[i] = 290000;
            }
            else if (temp >= 293 && temp < 297)
            {
                frequency[i] = 295000;
            }
            else
            {
                frequency[i] = 300000;
            }
        }
    }

    /* 分析结果输出-2 */
    // if (debug)
    {
        for (uint8_t i = 0; i < HARM_FREQ_NUM; i++)
        {
            bsprif1("place%d:%d \n", i, place[i]);
            bsprif1("frequency%d:%.2f \n", i, frequency[i]);
            bsprif1("ampl%d, %.2f\n ", i, ampl[i]);
        }
    }

    /* 谐波分量中
        1,2 -> 双波主频
        3,4 -> 区分正弦与三角
    */
    wave1.freq = frequency[0];
    wave2.freq = frequency[1];
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
        wave2.type = TRI;
    }
    else
    {
        wave2.type = SIN;
    }

    /* 分析结果输出-3 */
    bsprif1("\nwave 1 freq: %.2f \n", wave1.freq);
    if (wave1.type == SIN)
        bsprif1("wave 1 type: SIN \n");
    else
        bsprif1("wave 1 type: TRI \n");

    bsprif1("wave 2 freq: %.2f \n", wave2.freq);
    if (wave2.type == SIN)
        bsprif1("wave 2 type: SIN \n");
    else
        bsprif1("wave 2 type: TRI \n");
}
#endif
