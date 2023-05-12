/**
 * @file bsp_sccb.c
 * @author July (Email: JulyCub@163.com)
 * @brief SCCB communication protocol
 * @version 0.1
 * @date 2023.05.11
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_sccb.h"

/**
 * @brief 初始化SCCB相关引脚
 *
 */
void bsp_sccb_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    __BSP_RCC_GPIOx_CLK_ENABLE(BSP_SCCB_PORT);

    GPIO_InitStructure.Pin = BSP_SCCP_SCL | BSP_SCCP_SDA;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(BSP_SCCB_PORT, &GPIO_InitStructure);

    SCCB_SDA_OUT();
}

/**
 * @brief SCCB起始信号
 * @attention
 *  当时钟为高的时候,数据线的高到低,为SCCB起始信号
 *  在激活状态下,SDA和SCL均为低电平
 */
static void bsp_sccb_start(void)
{
    SCCB_SDA = 1;
    SCCB_SCL = 1;
    delay_us(50);
    SCCB_SDA = 0;
    delay_us(50);
    SCCB_SCL = 0; // 数据线恢复低电平，单操作函数必要
}

/**
 * @brief SCCB停止信号
 * @attention
 *  当时钟为高的时候,数据线的低到高,为SCCB停止信号
 *  空闲状况下,SDA,SCL均为高电平
 */
static void bsp_sccb_stop(void)
{
    SCCB_SDA = 0;
    delay_us(50);
    SCCB_SCL = 1;
    delay_us(50);
    SCCB_SDA = 1;
    delay_us(50);
}

/**
 * @brief 发送 SCCB NONE ACK 信号
 */
static void bsp_sccb_no_ack(void)
{
    delay_us(50);
    SCCB_SDA = 1;
    SCCB_SCL = 1;
    delay_us(50);
    SCCB_SCL = 0;
    delay_us(50);
    SCCB_SDA = 0;
    delay_us(50);
}

/**
 * @brief SCCB 写入一个字节
 * @param dat 数据
 * @return uint8_t 0-成功 1-失败
 */
static uint8_t bsp_sccb_WriteByte(uint8_t dat)
{
    uint8_t j, res;
    for (j = 0; j < 8; j++) // 循环8次发送数据
    {
        if (dat & 0x80)
            SCCB_SDA = 1;
        else
            SCCB_SDA = 0;
        dat <<= 1;
        delay_us(50);
        SCCB_SCL = 1;
        delay_us(50);
        SCCB_SCL = 0;
    }
    SCCB_SDA_IN(); // 设置SDA为输入
    delay_us(50);
    SCCB_SCL = 1; // 接收第九位,以判断是否发送成功
    delay_us(50);
    if (SCCB_READ_SDA)
        res = 1; // SDA=1发送失败，返回1
    else
        res = 0; // SDA=0发送成功，返回0
    SCCB_SCL = 0;
    SCCB_SDA_OUT(); // 设置SDA为输出
    return res;
}

/**
 * @brief SCCB 读取一个字节
 * @attention 在SCL的上升沿,数据锁存
 * @return uint8_t 读取到的数据
 */
static uint8_t bsp_sccb_ReadByte(void)
{
    uint8_t temp = 0, j;
    SCCB_SDA_IN();          // 设置SDA为输入
    for (j = 8; j > 0; j--) // 循环8次接收数据
    {
        delay_us(50);
        SCCB_SCL = 1;
        temp = temp << 1;
        if (SCCB_READ_SDA)
            temp++;
        delay_us(50);
        SCCB_SCL = 0;
    }
    SCCB_SDA_OUT(); // 设置SDA为输出
    return temp;
}

/**
 * @brief SCCB 写寄存器
 *
 * @param reg 寄存器地址
 * @param data 数据
 * @return uint8_t 0-失败 1-成功
 */
uint8_t bsp_sccb_WriteRegister(uint8_t reg, uint8_t data)
{
    uint8_t res = 0;
    bsp_sccb_start(); // 启动SCCB传输
    if (bsp_sccb_WriteByte(SCCB_ID))
        res = 1; // 写器件ID
    delay_us(100);
    if (bsp_sccb_WriteByte(reg))
        res = 1; // 写寄存器地址
    delay_us(100);
    if (bsp_sccb_WriteByte(data))
        res = 1; // 写数据
    bsp_sccb_stop();
    return res;
}

/**
 * @brief SCCB 读寄存器
 *
 * @param reg 寄存器地址
 * @return uint8_t 读取到的寄存器值
 */
uint8_t bsp_sccb_ReadRegister(uint8_t reg)
{
    uint8_t val = 0;
    bsp_sccb_start();            // 启动SCCB传输
    bsp_sccb_WriteByte(SCCB_ID); // 写器件ID
    delay_us(100);
    bsp_sccb_WriteByte(reg); // 写寄存器地址
    delay_us(100);
    bsp_sccb_stop();
    delay_us(100);
    // 设置寄存器地址后，才是读
    bsp_sccb_start();
    bsp_sccb_WriteByte(SCCB_ID | 0X01); // 发送读命令
    delay_us(100);
    val = bsp_sccb_ReadByte(); // 读取数据
    bsp_sccb_no_ack();
    bsp_sccb_stop();
    return val;
}
