/**
 * @file bsp_wtu404f4.c
 * @author SprInec (JulyCub@163.com)
 * @brief WTU404F4-B004 高精度红外模块
 * @version 0.1
 * @date 2025.01.08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "bsp_wtu404f4.h"

#if WTU404F4_MODE == WTU404F4_MODE_INT

/**
 * @brief 红外学习模式
 */
void BSP_WTU404F4_InfraredLearningMode(void)
{
    HAL_GPIO_WritePin(WTU404F4_DAT_PORT, WTU404F4_DAT_PIN, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(WTU404F4_DAT_PORT, WTU404F4_DAT_PIN, GPIO_PIN_SET);
}

#else
uint8_t BSP_WTU404F4_Init(void)
{
    uint8_t state = bspTrue;

    state &= BSP_WTU404F4_InfraredResponseSpeed(0x01);
    HAL_Delay(200);
    state &= BSP_WTU404F4_InfraredLearningMode();

    return state;
}

/**
 * @brief 红外学习模式
 */
uint8_t BSP_WTU404F4_InfraredLearningMode(void)
{
    uint8_t code[9];

    code[0] = WTU404F4_HEADER;
    code[1] = WTU404F4_HEADER;
    code[2] = 0x7E;
    code[3] = 0x05;
    code[4] = 0xFF;
    code[5] = 0x12;
    code[6] = 0xB1;
    code[7] = 0xC7;
    code[8] = 0xEF;
    __BSP_WTU404F4_SEND(code);

    return BSP_WTU404F4_AckData(code[6]);
}

/**
 * @brief 设置红外响应速度
 * @param speed 响应速度
 * - 0x01: 250ms
 * - 0x02: 500ms
 * - 0x03: 1000ms
 * - 0x04: 2000ms
 */
uint8_t BSP_WTU404F4_InfraredResponseSpeed(uint8_t speed)
{
    uint8_t code[10];

    code[0] = WTU404F4_HEADER;
    code[1] = WTU404F4_HEADER;
    code[2] = 0x7E;
    code[3] = 0x06;
    code[4] = 0xFF;
    code[5] = 0x12;
    code[6] = 0xB2;
    code[7] = speed;
    code[8] = (code[3] + code[4] + code[5] + code[6] + code[7]) & 0xFF;
    code[9] = 0xEF;
    __BSP_WTU404F4_SEND(code);

    return BSP_WTU404F4_AckData(code[6]);
}

/**
 * @brief 设置能量强度
 * @param intensity 0-100 能量强度
 */
uint8_t BSP_WTU404F4_InfraredEnergyIntensity(uint8_t intensity)
{
    uint8_t code[10];

    code[0] = WTU404F4_HEADER;
    code[1] = WTU404F4_HEADER;
    code[2] = 0x7E;
    code[3] = 0x06;
    code[4] = 0xFF;
    code[5] = 0x12;
    code[6] = 0xB3;
    code[7] = 0x00 | intensity;
    code[8] = (code[3] + code[4] + code[5] + code[6] + code[7]) & 0xFF;
    code[9] = 0xEF;
    __BSP_WTU404F4_SEND(code);

    return BSP_WTU404F4_AckData(code[6]);
}

/**
 * @brief INT 有效电平设置
 * @param level 
 * - 0x00:低电平有效 
 * - 0x01:高电平有效
 */
uint8_t BSP_WTU404F4_InfraredINT(uint8_t level)
{
    uint8_t code[10];

    code[0] = WTU404F4_HEADER;
    code[1] = WTU404F4_HEADER;
    code[2] = 0x7E;
    code[3] = 0x06;
    code[4] = 0xFF;
    code[5] = 0x12;
    code[6] = 0xB5;
    code[7] = level;
    code[8] = (code[3] + code[4] + code[5] + code[6] + code[7]) & 0xFF;
    code[9] = 0xEF;
    __BSP_WTU404F4_SEND(code);

    return BSP_WTU404F4_AckData(code[6]);
}

/**
 * @brief 有效数据主动上报
 * @param enable 
 */
uint8_t BSP_WTU404F4_InfraredDataReport(uint8_t enable)
{
    uint8_t code[10];

    code[0] = WTU404F4_HEADER;
    code[1] = WTU404F4_HEADER;
    code[2] = 0x7E;
    code[3] = 0x06;
    code[4] = 0xFF;
    code[5] = 0x12;
    code[6] = 0xB6;
    code[7] = enable;
    code[8] = (code[3] + code[4] + code[5] + code[6] + code[7]) & 0xFF;
    code[9] = 0xEF;
    __BSP_WTU404F4_SEND(code);

    return BSP_WTU404F4_AckData(code[6]);
}

/**
 * @brief WTU404F4 设置返回数据
 * @param oder 命令码
 * @return 
 * - bspTrue: 成功
 * - bspFalse: 失败
 */
uint8_t BSP_WTU404F4_AckData(uint8_t oder)
{
    uint8_t data[WTU404F4_RECLEN];
    uint8_t checkCode;

    __BSP_WTU404F4_RECEIVE(data);
    printf("WTU404F4_AckData:");
    for (uint8_t i = 0; i < WTU404F4_RECLEN; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");

    checkCode = (data[2] + data[3] + data[4] + data[5] + data[6]) & 0xFF;
    if ((oder == data[5]) && (checkCode == data[7]))
    {
        if (data[6] == 0x00)
        {
            return bspTrue;
        }
        else if (data[6] == 0x01)
        {
            return bspFalse;
        }
    }
    return bspFalse;
}

/**
 * @brief 红外感应监测数据
 * @param data 监测数据
 * @return 
 * - bspTrue: 有障碍
 * - bspFalse: 无障碍
 */
uint8_t BSP_WTU404F4_InfraredDetection(uint8_t data[])
{
    uint8_t checkCode;

    printf("BSP_WTU404F4_InfraredDetection:");
    for (uint8_t i = 0; i < WTU404F4_RECLEN; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
    checkCode = (data[2] + data[3] + data[4] + data[5] + data[6]) & 0xFF;
    if ((0xB6 == data[5]) && (checkCode == data[7]))
    {
        if (data[6] == 0x00)
        {
            return bspTrue;
        }
        else if (data[6] == 0x01)
        {
            return bspFalse;
        }
    }
    return bspFalse;
}

#endif /* WTU404F4_MODE */
