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
#include "gpio.h"

/**
 * @brief 初始化SCCB相关引脚
 *
 */
void BSP_SCCB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    __BSP_RCC_GPIOx_CLK_ENABLE(GPIOB);

    GPIO_InitStructure.Pin = BSP_SCCB_SCL | BSP_SCCB_SDA;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(BSP_SCCB_PORT, &GPIO_InitStructure);

    SCCB_SDA_OUT();
}

#if MCU_SELECTION <= 2
/**
 * @brief SCCB起始信号
 * @attention
 *  当时钟为高的时候,数据线的高到低,为SCCB起始信号
 *  在激活状态下,SDA和SCL均为低电平
 */
static void BSP_SCCB_Start(void)
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
static void BSP_SCCB_Stop(void)
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
static void BSP_SCCB_NAck(void)
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
static uint8_t BSP_SCCB_WriteByte(uint8_t dat)
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
static uint8_t BSP_SCCB_ReadByte(void)
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
#else
/**
 * @brief SCCB起始信号
 * @attention
 *  当时钟为高的时候,数据线的高到低,为SCCB起始信号
 *  在激活状态下,SDA和SCL均为低电平
 */
static void BSP_SCCB_Start(void)
{
    SCCB_SDA(1);
    SCCB_SCL(1);
    delay_us(50);
    SCCB_SDA(0);
    delay_us(50);
    SCCB_SCL(0); // 数据线恢复低电平，单操作函数必要
}

/**
 * @brief SCCB停止信号
 * @attention
 *  当时钟为高的时候,数据线的低到高,为SCCB停止信号
 *  空闲状况下,SDA,SCL均为高电平
 */
static void BSP_SCCB_Stop(void)
{
    SCCB_SDA(0);
    delay_us(50);
    SCCB_SCL(1);
    delay_us(50);
    SCCB_SDA(1);
    delay_us(50);
}

/**
 * @brief 发送 SCCB NONE ACK 信号
 */
static void BSP_SCCB_NAck(void)
{
    delay_us(50);
    SCCB_SDA(1);
    SCCB_SCL(1);
    delay_us(50);
    SCCB_SCL(0);
    delay_us(50);
    SCCB_SDA(0);
    delay_us(50);
}

/**
 * @brief SCCB 写入一个字节
 * @param dat 数据
 * @return uint8_t 0-成功 1-失败
 */
static uint8_t BSP_SCCB_WriteByte(uint8_t dat)
{
    uint8_t j, res;
    for (j = 0; j < 8; j++) // 循环8次发送数据
    {
        if (dat & 0x80)
            SCCB_SDA(1);
        else
            SCCB_SDA(0);
        dat <<= 1;
        delay_us(50);
        SCCB_SCL(1);
        delay_us(50);
        SCCB_SCL(0);
    }
    SCCB_SDA_IN(); // 设置SDA为输入
    delay_us(50);
    SCCB_SCL(1); // 接收第九位,以判断是否发送成功
    delay_us(50);
    if (SCCB_READ_SDA)
        res = 1; // SDA=1发送失败，返回1
    else
        res = 0; // SDA=0发送成功，返回0
    SCCB_SCL(0);
    SCCB_SDA_OUT(); // 设置SDA为输出
    return res;
}

/**
 * @brief SCCB 读取一个字节
 * @attention 在SCL的上升沿,数据锁存
 * @return uint8_t 读取到的数据
 */
static uint8_t BSP_SCCB_ReadByte(void)
{
    uint8_t temp = 0, j;
    SCCB_SDA_IN();          // 设置SDA为输入
    for (j = 8; j > 0; j--) // 循环8次接收数据
    {
        delay_us(50);
        SCCB_SCL(1);
        temp = temp << 1;
        if (SCCB_READ_SDA)
            temp++;
        delay_us(50);
        SCCB_SCL(0);
    }
    SCCB_SDA_OUT(); // 设置SDA为输出
    return temp;
}
#endif
/**
 * @brief SCCB 写寄存器
 *
 * @param reg 寄存器地址
 * @param data 数据
 * @return uint8_t 0-失败 1-成功
 */
uint8_t BSP_SCCB_WriteRegister(uint8_t reg, uint8_t data)
{
    uint8_t res = 0;
    BSP_SCCB_Start(); // 启动SCCB传输
    if (BSP_SCCB_WriteByte(SCCB_ID))
        res = 1; // 写器件ID
    delay_us(100);
    if (BSP_SCCB_WriteByte(reg))
        res = 1; // 写寄存器地址
    delay_us(100);
    if (BSP_SCCB_WriteByte(data))
        res = 1; // 写数据
    BSP_SCCB_Stop();
    return res;
}

/**
 * @brief SCCB 读寄存器
 *
 * @param reg 寄存器地址
 * @return uint8_t 读取到的寄存器值
 */
uint8_t BSP_SCCB_ReadRegister(uint8_t reg)
{
    uint8_t val = 0;
    BSP_SCCB_Start();            // 启动SCCB传输
    BSP_SCCB_WriteByte(SCCB_ID); // 写器件ID
    delay_us(100);
    BSP_SCCB_WriteByte(reg); // 写寄存器地址
    delay_us(100);
    BSP_SCCB_Stop();
    delay_us(100);

    // 设置寄存器地址后，才是读
    BSP_SCCB_Start();
    BSP_SCCB_WriteByte(SCCB_ID | 0X01); // 发送读命令
    delay_us(100);
    val = BSP_SCCB_ReadByte(); // 读取数据
    BSP_SCCB_NAck();
    BSP_SCCB_Stop();
    return val;
}
