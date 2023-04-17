/**
 ******************************************************************************
 * @file		:bsp_filter.c
 * @brief		:The board support package for filter.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.07.02
 ******************************************************************************
 */

#include "bsp_filter.h"

#if arm_dsp
#include "fdacoefs.h"

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = LENGTH_SAMPLES / BLOCK_SIZE; /* 需要调用arm_fir_f32的次数 */

float32_t Input_f32_50Hz_200Hz[LENGTH_SAMPLES];   /* 采样点 */
float32_t Output[LENGTH_SAMPLES];                 /* 滤波后的输出 */
float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; /* 状态缓存，大小numTaps + blockSize - 1*/
float32_t pCoeff[NUM_TAPS];                       /* 滤波因数(转置) */
#endif

#if arm_dsp
/*
******************************************************************************
*    函 数 名: bsp_arraytsp
*    功能说明: 数组倒置
*    形    参： arrA		需倒置的数组
*			    arrB        倒置后的数组
*				len         数组长度
*    返 回 值: 无
******************************************************************************
*/
void bsp_arraytsp(const real64_T arrA[], float32_t arrB[], uint16_t len)
{
    for (uint16_t i = 0, j = len - 1; i < len; i++, j--)
    {
        arrB[i] = arrA[j];
    }
}

void bsp_GettspB(void)
{
    bsp_arraytsp(B, pCoeff, NUM_TAPS);
}

/*
******************************************************************************
*    函 数 名: bsp_arm_fir_f32
*    功能说明: 调用函数arm_fir_f32_hp实现高通/低通/带通/带阻滤波器
*    形    参：无
*    返 回 值: 无
******************************************************************************
*/
void bsp_arm_fir_f32(void)
{
    uint32_t i;
    arm_fir_instance_f32 FIR_InitStruct;
    float32_t *inputF32, *outputF32;

    for (uint16_t i = 0; i < LENGTH_SAMPLES; i++)
    {
        Input_f32_50Hz_200Hz[i] = data_buff[i];
    }
    /* 初始化输入输出缓存指针 */
    inputF32 = &Input_f32_50Hz_200Hz[0];
    outputF32 = &Output[0];

    /* 初始化结构体FIR_InitStruct */
    arm_fir_init_f32(&FIR_InitStruct,
                     NUM_TAPS,
                     (float32_t *)&B[0],
                     &firStateF32[0],
                     blockSize);

    /* 实现FIR滤波，这里每次处理1个点 */
    for (i = 0; i < numBlocks; i++)
    {
        arm_fir_f32(&FIR_InitStruct, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
    }

#if 0
    /* 打印滤波后结果 */
    for(i=0; i<LENGTH_SAMPLES; i++)
    {
#if (USE_STDIO)
				printf(":%f, %fn", Output[i], inputF32[i]);
#else
				PRINTF_DEBUG(":%f, %f\n", Output[i], inputF32[i]);
#endif
    }
#endif
}
#endif

/*
******************************************************************************
*    函 数 名: Onepointfilter
*    功能说明: 单点滑动平均滤波
*    形    参：data		信息点
*    返 回 值: 无
******************************************************************************
*/
float Onepointfilter(int16_t data)
{
    static int16_t buf[FNUM];
    static int16_t index = 0, flag = 0;
    static float sum = 0;

    // 替换之前位置上的数据
    sum += data - buf[index];
    buf[index] = data;
    index++;

    // 控制数据循环放置到缓冲区
    if (index == FNUM)
    {
        index = 0;
        flag = 1;
    }

    // 如果没有充满缓存区，有几个就取几个的平均
    if (flag == 0)
        return sum / index;
    else
        return sum / FNUM;
}

/*
******************************************************************************
*    函 数 名: linearSmooth3
*    功能说明: 三点线性平滑滤波
*    形    参：in   输入数组
*							 out  输出数组
*							 N    数组长度
*    返 回 值: 无
******************************************************************************
*/
void linearSmooth3(double in[], double out[], int N)
{
    int i;
    if (N < 3)
    {
        for (i = 0; i <= N - 1; i++)
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = (5.0 * in[0] + 2.0 * in[1] - in[2]) / 6.0;

        for (i = 1; i <= N - 2; i++)
        {
            out[i] = (in[i - 1] + in[i] + in[i + 1]) / 3.0;
        }

        out[N - 1] = (5.0 * in[N - 1] + 2.0 * in[N - 2] - in[N - 3]) / 6.0;
    }
}

/*
******************************************************************************
*    函 数 名: linearSmooth5
*    功能说明: 五点线性平滑滤波
*    形    参：in   输入数组
*							 out  输出数组
*							 N    数组长度
*    返 回 值: 无
******************************************************************************
*/
void linearSmooth5(double in[], double out[], int N)
{
    int i;
    if (N < 5)
    {
        for (i = 0; i <= N - 1; i++)
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = (3.0 * in[0] + 2.0 * in[1] + in[2] - in[4]) / 5.0;
        out[1] = (4.0 * in[0] + 3.0 * in[1] + 2 * in[2] + in[3]) / 10.0;
        for (i = 2; i <= N - 3; i++)
        {
            out[i] = (in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2]) / 5.0;
        }
        out[N - 2] = (4.0 * in[N - 1] + 3.0 * in[N - 2] + 2 * in[N - 3] + in[N - 4]) / 10.0;
        out[N - 1] = (3.0 * in[N - 1] + 2.0 * in[N - 2] + in[N - 3] - in[N - 5]) / 5.0;
    }
}

/*
******************************************************************************
*    函 数 名: linearSmooth7
*    功能说明: 七点线性平滑滤波
*    形    参：in   输入数组
*							 out  输出数组
*							 N    数组长度
*    返 回 值: 无
******************************************************************************
*/
void linearSmooth7(double in[], double out[], int N)
{
    int i;
    if (N < 7)
    {
        for (i = 0; i <= N - 1; i++)
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = (13.0 * in[0] + 10.0 * in[1] + 7.0 * in[2] + 4.0 * in[3] +
                  in[4] - 2.0 * in[5] - 5.0 * in[6]) / 28.0;

        out[1] = (5.0 * in[0] + 4.0 * in[1] + 3 * in[2] + 2 * in[3] +
                  in[4] - in[6]) / 14.0;

        out[2] = (7.0 * in[0] + 6.0 * in[1] + 5.0 * in[2] + 4.0 * in[3] +
                  3.0 * in[4] + 2.0 * in[5] + in[6]) / 28.0;

        for (i = 3; i <= N - 4; i++)
        {
            out[i] = (in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3]) / 7.0;
        }

        out[N - 3] = (7.0 * in[N - 1] + 6.0 * in[N - 2] + 5.0 * in[N - 3] +
                      4.0 * in[N - 4] + 3.0 * in[N - 5] + 2.0 * in[N - 6] + in[N - 7]) / 28.0;

        out[N - 2] = (5.0 * in[N - 1] + 4.0 * in[N - 2] + 3.0 * in[N - 3] +
                      2.0 * in[N - 4] + in[N - 5] - in[N - 7]) / 14.0;

        out[N - 1] = (13.0 * in[N - 1] + 10.0 * in[N - 2] + 7.0 * in[N - 3] +
                      4 * in[N - 4] + in[N - 5] - 2 * in[N - 6] - 5 * in[N - 7]) / 28.0;
    }
}

/*
******************************************************************************
*    函 数 名: quadraticSmooth5
*    功能说明: 五点二次平滑滤波
*    形    参：in   输入数组
*							 out  输出数组
*							 N    数组长度
*    返 回 值: 无
******************************************************************************
*/
void quadraticSmooth5(double in[], double out[], int N)
{
    int i;
    if (N < 5)
    {
        for (i = 0; i <= N - 1; i++)
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = (31.0 * in[0] + 9.0 * in[1] - 3.0 * in[2] - 5.0 * in[3] + 3.0 * in[4]) / 35.0;
        out[1] = (9.0 * in[0] + 13.0 * in[1] + 12 * in[2] + 6.0 * in[3] - 5.0 * in[4]) / 35.0;
        for (i = 2; i <= N - 3; i++)
        {
            out[i] = (-3.0 * (in[i - 2] + in[i + 2]) +
                      12.0 * (in[i - 1] + in[i + 1]) + 17 * in[i]) / 35.0;
        }
        out[N - 2] = (9.0 * in[N - 1] + 13.0 * in[N - 2] + 12.0 * in[N - 3] + 6.0 * in[N - 4] - 5.0 * in[N - 5]) / 35.0;
        out[N - 1] = (31.0 * in[N - 1] + 9.0 * in[N - 2] - 3.0 * in[N - 3] - 5.0 * in[N - 4] + 3.0 * in[N - 5]) / 35.0;
    }
}

/*
******************************************************************************
*    函 数 名: quadraticSmooth7
*    功能说明: 七点二次平滑滤波
*    形    参：in   输入数组
*							 out  输出数组
*							 N    数组长度
*    返 回 值: 无
******************************************************************************
*/
void quadraticSmooth7(double in[], double out[], int N)
{
    int i;
    if (N < 7)
    {
        for (i = 0; i <= N - 1; i++)
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = (32.0 * in[0] + 15.0 * in[1] + 3.0 * in[2] - 4.0 * in[3] -
                  6.0 * in[4] - 3.0 * in[5] + 5.0 * in[6]) / 42.0;

        out[1] = (5.0 * in[0] + 4.0 * in[1] + 3.0 * in[2] + 2.0 * in[3] +
                  in[4] - in[6]) / 14.0;

        out[2] = (1.0 * in[0] + 3.0 * in[1] + 4.0 * in[2] + 4.0 * in[3] +
                  3.0 * in[4] + 1.0 * in[5] - 2.0 * in[6]) / 14.0;
        for (i = 3; i <= N - 4; i++)
        {
            out[i] = (-2.0 * (in[i - 3] + in[i + 3]) +
                      3.0 * (in[i - 2] + in[i + 2]) +
                      6.0 * (in[i - 1] + in[i + 1]) + 7.0 * in[i]) / 21.0;
        }
        out[N - 3] = (1.0 * in[N - 1] + 3.0 * in[N - 2] + 4.0 * in[N - 3] +
                      4.0 * in[N - 4] + 3.0 * in[N - 5] + 1.0 * in[N - 6] - 2.0 * in[N - 7]) / 14.0;

        out[N - 2] = (5.0 * in[N - 1] + 4.0 * in[N - 2] + 3.0 * in[N - 3] +
                      2.0 * in[N - 4] + in[N - 5] - in[N - 7]) / 14.0;

        out[N - 1] = (32.0 * in[N - 1] + 15.0 * in[N - 2] + 3.0 * in[N - 3] -
                      4.0 * in[N - 4] - 6.0 * in[N - 5] - 3.0 * in[N - 6] + 5.0 * in[N - 7]) / 42.0;
    }
}

/*
******************************************************************************
*    函 数 名: cubicSmooth5
*    功能说明: 五点三次平滑滤波
*    形    参：in   输入数组
*							 out  输出数组
*							 N    数组长度
*    返 回 值: 无
******************************************************************************
*/
void cubicSmooth5(double in[], double out[], int N)
{

    int i;
    if (N < 5)
    {
        for (i = 0; i <= N - 1; i++)
            out[i] = in[i];
    }

    else
    {
        out[0] = (69.0 * in[0] + 4.0 * in[1] - 6.0 * in[2] + 4.0 * in[3] - in[4]) / 70.0;
        out[1] = (2.0 * in[0] + 27.0 * in[1] + 12.0 * in[2] - 8.0 * in[3] + 2.0 * in[4]) / 35.0;
        for (i = 2; i <= N - 3; i++)
        {
            out[i] = (-3.0 * (in[i - 2] + in[i + 2]) + 12.0 * (in[i - 1] + in[i + 1]) + 17.0 * in[i]) / 35.0;
        }
        out[N - 2] = (2.0 * in[N - 5] - 8.0 * in[N - 4] + 12.0 * in[N - 3] + 27.0 * in[N - 2] + 2.0 * in[N - 1]) / 35.0;
        out[N - 1] = (-in[N - 5] + 4.0 * in[N - 4] - 6.0 * in[N - 3] + 4.0 * in[N - 2] + 69.0 * in[N - 1]) / 70.0;
    }
    return;
}

/*
******************************************************************************
*    函 数 名: cubicSmooth7
*    功能说明: 七点三次平滑滤波
*    形    参：in   输入数组
*							 out  输出数组
*							 N    数组长度
*    返 回 值: 无
******************************************************************************
*/
void cubicSmooth7(double in[], double out[], int N)
{
    int i;
    if (N < 7)
    {
        for (i = 0; i <= N - 1; i++)
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = (39.0 * in[0] + 8.0 * in[1] - 4.0 * in[2] - 4.0 * in[3] +
                  1.0 * in[4] + 4.0 * in[5] - 2.0 * in[6]) / 42.0;
        out[1] = (8.0 * in[0] + 19.0 * in[1] + 16.0 * in[2] + 6.0 * in[3] -
                  4.0 * in[4] - 7.0 * in[5] + 4.0 * in[6]) / 42.0;
        out[2] = (-4.0 * in[0] + 16.0 * in[1] + 19.0 * in[2] + 12.0 * in[3] +
                  2.0 * in[4] - 4.0 * in[5] + 1.0 * in[6]) / 42.0;
        for (i = 3; i <= N - 4; i++)
        {
            out[i] = (-2.0 * (in[i - 3] + in[i + 3]) +
                      3.0 * (in[i - 2] + in[i + 2]) +
                      6.0 * (in[i - 1] + in[i + 1]) + 7.0 * in[i]) / 21.0;
        }
        out[N - 3] = (-4.0 * in[N - 1] + 16.0 * in[N - 2] + 19.0 * in[N - 3] +
                      12.0 * in[N - 4] + 2.0 * in[N - 5] - 4.0 * in[N - 6] + 1.0 * in[N - 7]) / 42.0;
        out[N - 2] = (8.0 * in[N - 1] + 19.0 * in[N - 2] + 16.0 * in[N - 3] +
                      6.0 * in[N - 4] - 4.0 * in[N - 5] - 7.0 * in[N - 6] + 4.0 * in[N - 7]) / 42.0;
        out[N - 1] = (39.0 * in[N - 1] + 8.0 * in[N - 2] - 4.0 * in[N - 3] -
                      4.0 * in[N - 4] + 1.0 * in[N - 5] + 4.0 * in[N - 6] - 2.0 * in[N - 7]) / 42.0;
    }
}