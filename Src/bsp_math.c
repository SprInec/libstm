/**
 ******************************************************************************
 * @file			:bsp_math.c
 * @brief			:some math function.
 * @version		    :0.1.0
 * @author		    :July
 * @date			:2022.07.05
 ******************************************************************************
 */

#include "bsp_math.h"

/**
 * @brief 正弦码表生成器
 * @param points 点数 
 * @param result 码表
 */
void bsp_sin(uint16_t points, float *result)
{
    for (uint16_t i = 0; i < points; i++)
        *(result + i) = sin((float)(PI / (points / 2.0f)) * i);
}

/**
 * @brief 自相关函数
 * 
 * @param data 待计算数组
 * @param m 起始滞后下标位置
 * @param N 滞后范围
 * @return float 
 */
float bsp_xcorr(float *data, int m, int N)
{
    float r = 0.0;

    for (int i = m; i < N; i++)
    {
        r += data[i] * data[i - m];
    }
    return r;
}



