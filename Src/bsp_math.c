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

/**
 * @brief 向上取整
 * @param num 带计算数据
 * @return uint16_t 结果
 */
uint16_t bsp_upint(float num)
{
    if (num > (uint16_t)num)
        return (uint16_t)num + 1;
    else
        return (uint16_t)num;
}

/**
 * @brief 向下取整
 * @param num 待计算数据
 * @return uint16_t 结果
 */
uint16_t bsp_downint(float num)
{
    return (uint16_t)num;
}

/**
 * @brief 数组倒置
 * @param arrA 需倒置的数组
 * @param arrB 倒置后的数组
 * @param len 数组长度
 */
void bsp_arraytsp(const real64_T arrA[], float32_t arrB[], uint16_t len)
{
    for (uint16_t i = 0, j = len - 1; i < len; i++, j--)
    {
        arrB[i] = arrA[j];
    }
}
