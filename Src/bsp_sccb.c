/**
 * @file bsp_sccb.c
 * @author July (Email: JulyCub@163.com)
 * @brief SCCB Driver
 * @version 0.1
 * @date 2023.07.16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_sccb.h"
#include "bsp_delay.h"
#include "gpio.h"

static void SCCB_Delay(void)
{
	u16 i;
	for (i = 0; i < 0xFF; i++);
}

static void SCCB_Delay_us(u16 D_Time)
{
	u16 i;
	for (i = 0; i < D_Time; i++);
}

/**
 * @brief SCCB 初始化
 */
void SCCB_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE(); // 使能GPIOB时钟

	// PB10--SCL PB11--SDA  初始化设置
	GPIO_Initure.Pin = GPIO_PIN_10 | GPIO_PIN_11;

	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;		// 推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;				// 上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 快速
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);
}

/**
 * @brief SCCB SDA 设置为输入
 */
void SCCB_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	// PB10--SCL PB11--SDA  初始化设置
	GPIO_Initure.Pin = GPIO_PIN_11;
	GPIO_Initure.Mode = GPIO_MODE_INPUT;			// 输入模式
	GPIO_Initure.Pull = GPIO_PULLUP;				// 上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 快速
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);
}

/**
 * @brief SCCB SDA 设置为输出
 */
void SCCB_SDA_OUT(void)
{

	GPIO_InitTypeDef GPIO_Initure;

	GPIO_Initure.Pin = GPIO_PIN_11;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;		// 推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;				// 上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 快速
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);
}

/**
 * @brief SCCB起始信号
 * @details 当时钟为高的时候,数据线的高到低,为SCCB起始信号
 * 			在激活状态下,SDA和SCL均为低电平
 */
void SCCB_Start(void)
{
	SCCB_SDA(1); // 数据线高电平
	SCCB_SCL(1); // 在时钟线高的时候数据线由高至低
	SCCB_Delay();
	SCCB_SDA(0);
	SCCB_Delay();
	SCCB_SCL(0); // 数据线恢复低电平，单操作函数必要
}

/**
 * @brief SCCB停止信号
 * @details 当时钟为高的时候,数据线的低到高,为SCCB停止信号
 * 			空闲状况下,SDA,SCL均为高电平
 */
void SCCB_Stop(void)
{
	SCCB_SDA(0);
	SCCB_Delay();
	SCCB_SCL(1);
	SCCB_Delay();
	SCCB_SDA(1);
	SCCB_Delay();
}

/**
 * @brief 产生NA信号
 */
void SCCB_No_Ack(void)
{
	SCCB_Delay();
	SCCB_SDA(1);
	SCCB_SCL(1);
	SCCB_Delay();
	SCCB_SCL(0);
	SCCB_Delay();
	SCCB_SDA(0);
	SCCB_Delay();
}

/**
 * @brief SCCB 写入一个字节
 * @param dat 数据
 * @return u8 0-成功 1-失败
 */
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j, res;
	for (j = 0; j < 8; j++) // 循环8次发送数据
	{
		if (dat & 0x80)
			SCCB_SDA(1);
		else
			SCCB_SDA(0);
		dat <<= 1;
		SCCB_Delay();
		SCCB_SCL(1);
		SCCB_Delay();
		SCCB_SCL(0);
	}
	SCCB_SDA_IN(); // 设置SDA为输入
	SCCB_Delay();
	SCCB_SCL(1); // 接收第九位,以判断是否发送成功
	SCCB_Delay();
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
 * @details 在SCL的上升沿,数据锁存
 * @return u8 读取到的数据
 */
u8 SCCB_RD_Byte(void)
{
	u8 temp = 0, j;
	SCCB_SDA_IN();			// 设置SDA为输入
	for (j = 8; j > 0; j--) // 循环8次接收数据
	{
		SCCB_Delay();
		SCCB_SCL(1);
		temp = temp << 1;
		if (SCCB_READ_SDA)
			temp++;
		SCCB_Delay();
		SCCB_SCL(0);
	}
	SCCB_SDA_OUT(); // 设置SDA为输出
	return temp;
}

/**
 * @brief SCCB 写寄存器
 * @param reg 寄存器地址
 * @param data 数据
 * @return u8 0-成功 1-失败
 */
u8 SCCB_WR_Reg(u8 reg, u8 data)
{
	u8 res = 0;
	SCCB_Start(); // 启动SCCB传输
	if (SCCB_WR_Byte(SCCB_ID))
		res = 1; // 写器件ID
	SCCB_Delay_us(100);
	if (SCCB_WR_Byte(reg))
		res = 1; // 写寄存器地址
	SCCB_Delay_us(100);
	if (SCCB_WR_Byte(data))
		res = 1; // 写数据
	SCCB_Stop();
	return res;
}

/**
 * @brief 读寄存器
 * @param reg 寄存器地址
 * @return u8 读取的寄存器值
 */
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val = 0;
	SCCB_Start();		   // 启动SCCB传输
	SCCB_WR_Byte(SCCB_ID); // 写器件ID
	SCCB_Delay_us(100);
	SCCB_WR_Byte(reg); // 写寄存器地址
	SCCB_Delay_us(100);
	SCCB_Stop();
	SCCB_Delay_us(100);
	// 设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID | 0X01); // 发送读命令
	SCCB_Delay_us(100);
	val = SCCB_RD_Byte(); // 读取数据
	SCCB_No_Ack();
	SCCB_Stop();
	return val;
}
