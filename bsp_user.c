/**
 * @file bsp_user.c
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023.07.13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_user.h"

BSP_SPCDDS_TypeDef dds = {
    .mode = 0,
    .freq= 0.0f,
    .ampl = 0.0f
};

/**
 * @brief BSP 初始化
 */
void BSP_Init(void)
{
    BSP_LED_Init();
    BSP_PE4302_Init();
    BSP_ADF4351_Init();
    ADF4351WriteFreq(10);
}

/**
 * @brief 串口接收回调函数
 * @param str 接收到的数据
 */
void BSP_UsartVar_Callback(uint8_t *str)
{
    /* 获取头部信息 */
    uint8_t head = *(str + 0);
    switch (head)
    {
    case /* FREQ -> HZ */  'A':
        sscanf((const char *)str, "A%f", &dds.freq);
        ADF4351WriteFreq(dds.freq / 1000000);
        printf("A-%f\n", dds.freq);
        break;
    case /* FREQ -> kHz */ 'B':
        sscanf((const char *)str, "B%f", &dds.freq);
        dds.freq *= 1000;
        ADF4351WriteFreq(dds.freq / 1000000);
        printf("B-%f\n", dds.freq);
        break;
    case /* FREQ -> MHz */ 'C':
        sscanf((const char *)str, "C%f", &dds.freq);
        dds.freq *= 1000000;
        ADF4351WriteFreq(dds.freq / 1000000);
        printf("C-%f\n", dds.freq);
        break;
    case /* FREQ -> GHz */ 'D':
        sscanf((const char *)str, "D%f", &dds.freq);
        dds.freq *= 1000000000;
        ADF4351WriteFreq(dds.freq / 1000000);
        printf("D-%f\n", dds.freq);
        break;
    case /* AMPL -> V */   'E':
        sscanf((const char *)str, "E%f", &dds.ampl);
        dds.ampl *= 1000;
        printf("E-%f\n", dds.freq);
        break;
    case /* AMPL -> mV */  'F':
        sscanf((const char *)str, "F%f", &dds.ampl);
        printf("F-%f\n", dds.freq);
        break;
    case /* AM */ 'M':
        dds.mode = 1;
        printf("M-AM\n");
        break;
    case /* FM */ 'N':
        dds.mode = 0;
        printf("N-FM\n");
        break;
    default:
        break;
    }
}

/**
 * @brief 串口接收控制程序
 */
void BSP_UART_CONTR(void)
{
    BSP_UsartVar_Conduct();
}