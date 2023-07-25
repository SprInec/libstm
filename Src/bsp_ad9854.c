/**
 * @file bsp_ad9854.h
 * @author July (Email: JulyCub@163.com)
 * @brief AD9854 Driver
 * @version 0.1
 * @date 2023.07.24
 * @pinset
 *          --
 *          RESET   - PE4;
 *          UDCLK   - PC13;
 *          WR      - PE3;
 *          RD      - PE2;
 *          OSK     - PE6;
 *          FSK	  	- PE5;
 *          D0		- PF0;
 *          D1		- PF1;
 *          D2		- PF2;
 *          D3		- PF3;
 *          D4		- PF4;
 *          D5		- PF5;
 *          D6		- PF6;
 *          D7		- PF7;
 *          A0		- PF8;
 *          A1		- PF9;
 *          A2		- PF10;
 *          A3		- PF11;
 *          A4		- PF12;
 *          A5		- PF13;
 *          VDD     逻辑电源(3.3V)
 *          VSS     GND(0V)
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_ad9854.h"
#include "bsp_delay.h"
#include "gpio.h"

/* 6个字节频率控制字 */
uchar FreqWord[6];

//**********************以下为系统时钟以及其相关变量设置**************************

/*
		此处根据自己的需要设置系统时钟以及与其相关的因子，一次需且只需开启一个
		CLK_Set为时钟倍频设置，可设置4~20倍倍频，但最大不能超过300MHZ
		Freq_mult_ulong和Freq_mult_doulle均为2的48次方除以系统时钟，一个为长整形，一个为双精度型

		AD9854板载晶振为20MHZ，设置15倍频，即SYSCLK=300MHZ

*/

/*
#define      CLK_Set            4
const ulong  Freq_mult_ulong  = 3518437;
const double Freq_mult_doulle = 3518437.2088832;
*/

/*
#define      CLK_Set            5
const ulong  Freq_mult_ulong  = 2814750;
const double Freq_mult_doulle = 2814749.76710656;
*/

/*
#define      CLK_Set            6
const ulong  Freq_mult_ulong  = 2345625;
const double Freq_mult_doulle = 2345624.80592213;
 */

/*
#define      CLK_Set            7
const ulong  Freq_mult_ulong  = 2010536;
const double Freq_mult_doulle = 2010535.54793326;
*/

/*
#define      CLK_Set            8
const ulong  Freq_mult_ulong  = 1759219;
const double Freq_mult_doulle = 1759218.6044416;
*/

/*
#define      CLK_Set            9
const ulong  Freq_mult_ulong  = 1563750;
const double Freq_mult_doulle = 1563749.87061476;
*/

/*
#define      CLK_Set            10
const ulong  Freq_mult_ulong  = 1407375;
const double Freq_mult_doulle = 1407374.88355328;
*/

/*
#define      CLK_Set            11
const ulong  Freq_mult_ulong  = 1279432;
const double Freq_mult_doulle = 1279431.712321164;
*/

/*
#define      CLK_Set            12
const ulong  Freq_mult_ulong  = 1172812;
const double Freq_mult_doulle = 1172812.402961067;
*/

/*
#define      CLK_Set            13
const ulong  Freq_mult_ulong  = 1082596;
const double Freq_mult_doulle = 1082596.064271754;
*/

/*
#define      CLK_Set            14
const ulong  Freq_mult_ulong  = 1005268;
const double Freq_mult_doulle = 1005267.773966629;
*/

#define CLK_Set 15
const ulong Freq_mult_ulong = 938250;
const double Freq_mult_doulle = 938249.9223688533;

/************************************************************
** 函数名称 ：void AD9854_IO_Init(void)
** 函数功能 ：控制AD9854需要用到的IO口在此初始化
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9854_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitStructure.Pin = GPIO_PIN_13;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
							 GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 |
							 GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/************************************************************
** 函数名称 ：void AD9854_WR_Byte(u32 addr,u32 dat)
** 函数功能 ：AD9854并行口写入数据
** 入口参数 ：addr     6位地址
			  dat      写入的数据
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
u32 dat1, dat2;
void AD9854_WR_Byte(u32 addr, u32 dat)
{
	dat1 = dat | (addr << 8);

	AD9854_AdrBus = (dat1 & 0x3fff) | ((dat1 ^ 0x3fff) << 16);

	//	AD9854_AdrBus  = ((addr&0x3f)<<8) | ((addr^0x3f)<<(16+8));
	//	AD9854_DataBus = dat|(dat^0xff)<<16;
	//	dat1  = ((addr&0x3f)<<8) | ((addr^0x3f)<<(16+8));
	//	dat2 = (dat1&0x3fff)|((dat1^0x3fff)<<16);
	AD9854_WR = 0;
	AD9854_WR = 1;
}

/************************************************************
** 函数名称 ：void Freq_convert(long Freq)
** 函数功能 ：正弦信号频率数据转换
** 入口参数 ：Freq，需要转换的频率，取值从0~SYSCLK/2
** 出口参数 ：无，但是影响全局变量FreqWord[6]的值
** 函数说明 ：该算法位多字节相乘算法，有公式FTW = (Desired Output Frequency × 2N)/SYSCLK
			  得到该算法，其中N=48，Desired Output Frequency 为所需要的频率，即Freq，SYSCLK
			  为可编程的系统时钟，FTW为48Bit的频率控制字，即FreqWord[6]
**************************************************************/
void Freq_convert(long Freq)
{
	ulong FreqBuf;
	ulong Temp = Freq_mult_ulong;

	uchar Array_Freq[4]; // 将输入频率因子分为四个字节
	Array_Freq[0] = (uchar)Freq;
	Array_Freq[1] = (uchar)(Freq >> 8);
	Array_Freq[2] = (uchar)(Freq >> 16);
	Array_Freq[3] = (uchar)(Freq >> 24);

	FreqBuf = Temp * Array_Freq[0];
	FreqWord[0] = FreqBuf;
	FreqBuf >>= 8;

	FreqBuf += (Temp * Array_Freq[1]);
	FreqWord[1] = FreqBuf;
	FreqBuf >>= 8;

	FreqBuf += (Temp * Array_Freq[2]);
	FreqWord[2] = FreqBuf;
	FreqBuf >>= 8;

	FreqBuf += (Temp * Array_Freq[3]);
	FreqWord[3] = FreqBuf;
	FreqBuf >>= 8;

	FreqWord[4] = FreqBuf;
	FreqWord[5] = FreqBuf >> 8;
}

/************************************************************
** 函数名称 ：void Freq_double_convert(double Freq)
** 函数功能 ：正弦信号频率数据转换
** 入口参数 ：Freq，需要转换的频率，取值从0~SYSCLK/2
** 出口参数 ：无，但是影响全局变量FreqWord[6]的值
** 函数说明 ：有公式FTW = (Desired Output Frequency × 2N)/SYSCLK得到该函数，
			  其中N=48，Desired Output Frequency 为所需要的频率，即Freq，SYSCLK
			  为可编程的系统时钟，FTW为48Bit的频率控制字，即FreqWord[6]
	注意：    该函数与上面函数的区别为该函数的入口参数为double，可使信号的频率更精确
			  建议在100HZ以下用本函数，在高于100HZ的情况下用函数void Freq_convert(long Freq)
**************************************************************/
void Freq_double_convert(double Freq)
{
	ulong Low32;
	uint High16;
	double Temp = Freq_mult_doulle; // 为2的48次方除以300M
	Freq *= (double)(Temp);
	//	1 0000 0000 0000 0000 0000 0000 0000 0000 = 4294967295
	High16 = (int)(Freq / 4294967295); // 2^32 = 4294967295
	Freq -= (double)High16 * 4294967295;
	Low32 = (ulong)Freq;

	FreqWord[0] = Low32;
	FreqWord[1] = Low32 >> 8;
	FreqWord[2] = Low32 >> 16;
	FreqWord[3] = Low32 >> 24;
	FreqWord[4] = High16;
	FreqWord[5] = High16 >> 8;
}

/************************************************************
** 函数名称 ：void AD9854_InitSingle(void)
** 函数功能 ：AD9854点频模式初始化
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9854_InitSingle(void)
{
	AD9854_WR = 1; // 将读、写控制端口设为无效
	AD9854_RD = 1;
	AD9854_UDCLK = 0;
	AD9854_RST = 1; // 复位AD9854
	AD9854_RST = 0;

	AD9854_WR_Byte(0x1d, 0x00); // 开启比较器
	//	AD9854_WR_Byte(0x1d,0x10);	       //关闭比较器
	AD9854_WR_Byte(0x1e, CLK_Set); // 设置系统时钟倍频
	AD9854_WR_Byte(0x1f, 0x00);	   // 设置系统为模式0点频，由外部更新
	AD9854_WR_Byte(0x20, 0x60);	   // 设置为可调节幅度，取消插值补偿

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_SetSine(ulong Freq,uint Shape)
** 函数功能 ：AD9854正弦波产生程序,频率为整数
** 入口参数 ：Freq   频率设置，取值范围为0~(1/2)*SYSCLK
			 Shape  幅度设置. 为12 Bit,取值范围为(0~4095) ,取值越大,幅度越大
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9854_SetSine(ulong Freq, uint Shape)
{
	uchar count;
	uchar Adress;

	Adress = 0x04; // 选择频率控制字地址的初值

	Freq_convert(Freq); // 频率转换

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress++, FreqWord[--count]);
	}

	AD9854_WR_Byte(0x21, Shape >> 8); // 设置I通道幅度
	AD9854_WR_Byte(0x22, (uchar)(Shape & 0xff));

	AD9854_WR_Byte(0x23, Shape >> 8); // 设置Q通道幅度
	AD9854_WR_Byte(0x24, (uchar)(Shape & 0xff));

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_SetSine_double(double Freq,uint Shape)
** 函数功能 ：AD9854正弦波产生程序,频率可为小数
** 入口参数 ：Freq   频率设置，取值范围为0~(1/2)*SYSCLK
							Shape  幅度设置. 为12 Bit,取值范围为(0~4095) ,取值越大,幅度越大
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9854_SetSine_double(double Freq, uint Shape)
{
	uchar count = 0;
	uchar Adress;

	Adress = 0x04; // 选择频率控制字1地址的初值

	Freq_double_convert(Freq); // 频率转换

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress++, FreqWord[--count]);
	}

	AD9854_WR_Byte(0x21, Shape >> 8); // 设置I通道幅度
	AD9854_WR_Byte(0x22, (uchar)(Shape & 0xff));

	AD9854_WR_Byte(0x23, Shape >> 8); // 设置Q通道幅度
	AD9854_WR_Byte(0x24, (uchar)(Shape & 0xff));

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_InitFSK(void)
** 函数功能 ：AD9854的FSK模式初始化
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9854_InitFSK(void)
{
	AD9854_WR = 1; // 将读、写控制端口设为无效
	AD9854_RD = 1;
	AD9854_UDCLK = 0;
	AD9854_RST = 1; // 复位AD9854
	AD9854_RST = 0;

	AD9854_WR_Byte(0x1d, 0x00); // 开启比较器
	//	AD9854_WR_Byte(0x1d,0x10);	       //关闭比较器
	AD9854_WR_Byte(0x1e, CLK_Set); // 设置系统时钟倍频
	AD9854_WR_Byte(0x1f, 0x02);	   // 设置系统为模式1,FSK，由外部更新
	AD9854_WR_Byte(0x20, 0x60);	   // 设置为可调节幅度，取消插值补偿

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_SetFSK(ulong Freq1,ulong Freq2)
** 函数功能 ： AD9854的FSK参数设置
** 入口参数 ：Freq1   FSK频率1
			  Freq2   FSK频率2
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9854_SetFSK(ulong Freq1, ulong Freq2)
{
	uchar count = 6;
	uchar Adress1, Adress2;

	const uint Shape = 4000; // 幅度设置. 为12 Bit,取值范围为(0~4095)

	Adress1 = 0x04; // 选择频率控制字1地址的初值
	Adress2 = 0x0a; // 选择频率控制字2地址的初值

	Freq_convert(Freq1); // 频率转换1

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress1++, FreqWord[--count]);
	}

	Freq_convert(Freq2); // 频率转换2

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress2++, FreqWord[--count]);
	}

	AD9854_WR_Byte(0x21, Shape >> 8); // 设置I通道幅度
	AD9854_WR_Byte(0x22, (uchar)(Shape & 0xff));

	AD9854_WR_Byte(0x23, Shape >> 8); // 设置Q通道幅度
	AD9854_WR_Byte(0x24, (uchar)(Shape & 0xff));

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_InitRFSK(void)
** 函数功能 ：AD9854的RFSK模式初始化 线性调频模式即扫频
** 入口参数 ：autoSweepEn：AUTO,自动扫频，
			此时无需外部控制,芯片自动从下限频率扫描到上限频率，再自动从上限频率扫描到下限频率，以此自动循环，实现上下双方向自动循环扫频
			autoSweepEn：MANUAL,AD9854_FSK_BPSK_HOLD控制脚控制扫频
			此时需外部引脚FSK脚控制扫频，不能自动扫频，扫频方向取决于FSK逻辑电平
			AD9854_FSK_BPSK_HOLD引脚置高(3.3V)，AD9854从下限频率扫描到上限频率，扫描完成后保持在上限频率
			AD9854_FSK_BPSK_HOLD引脚置低(  0V)，AD9854从上限频率扫描到下限频率，扫描完成后保持在下限频率

** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9854_InitRFSK(uchar autoSweepEn)
{
	AD9854_WR = 1; // 将读、写控制端口设为无效
	AD9854_RD = 1;
	AD9854_UDCLK = 0;
	AD9854_RST = 1; // 复位AD9854
	AD9854_RST = 0;

	AD9854_WR_Byte(0x1d, 0x00); // 开启比较器
	//	AD9854_WR_Byte(0x1d,0x10);	       //关闭比较器
	AD9854_WR_Byte(0x1e, CLK_Set); // 设置系统时钟倍频
	if (autoSweepEn == AUTO)
		AD9854_WR_Byte(0x1f, 0x24); // 设置系统为模式2,RFSK，使能三角波扫频功能
	if (autoSweepEn == MANUAL)
		AD9854_WR_Byte(0x1f, 0x04); // 设置系统为模式2,RFSK
	AD9854_WR_Byte(0x20, 0x60);		// 设置为可调节幅度，取消插值补偿

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_SetRFSK(void)
** 函数功能 ：AD9854的RFSK扫频参数设置
** 入口参数 ：Freq_Low						RFSK低频率	   48Bit 范围：1Hz~120000000Hz(120M Hz)
							Freq_High					RFSK高频率	   48Bit 范围：1Hz~120000000Hz(120M Hz)
							Freq_Up_Down			步进频率	  	 48Bit 范围：1Hz~120000000Hz(120M Hz)
							FreRate						斜率时钟控制  20Bit 范围：0~1048575
** 出口参数 ：无
** 函数说明 ：需要保证，Freq_Low<Freq_High
							频点与频点间停留时间 dt_P = (FreRate+1) * (SYSCLK) ；例：FreRate=1048575，则dt_P=3.495mS
							扫频总时间=总扫描频点数*dt_P
**************************************************************/
void AD9854_SetRFSK(ulong Freq_Low, ulong Freq_High, ulong Freq_Up_Down, ulong FreRate)
{
	uchar count = 6;
	uchar Adress1, Adress2, Adress3;
	const uint Shape = 4000; // 幅度设置. 为12 Bit,取值范围为(0~4095)

	Adress1 = 0x04; // 选择频率控制字地址的初值
	Adress2 = 0x0a;
	Adress3 = 0x10;

	Freq_convert(Freq_Low); // 频率1转换

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress1++, FreqWord[--count]);
	}

	Freq_convert(Freq_High); // 频率2转换

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress2++, FreqWord[--count]);
	}

	Freq_convert(Freq_Up_Down); // 步进频率转换

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress3++, FreqWord[--count]);
	}

	AD9854_WR_Byte(0x1a, (uchar)((FreRate >> 16) & 0x0f)); // 设置斜升速率
	AD9854_WR_Byte(0x1b, (uchar)(FreRate >> 8));
	AD9854_WR_Byte(0x1c, (uchar)FreRate);

	AD9854_WR_Byte(0x21, Shape >> 8); // 设置I通道幅度
	AD9854_WR_Byte(0x22, (uchar)(Shape & 0xff));

	AD9854_WR_Byte(0x23, Shape >> 8); // 设置Q通道幅度
	AD9854_WR_Byte(0x24, (uchar)(Shape & 0xff));

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_InitChirp(void)
** 函数功能 ：AD9854的Chirp模式初始化 线性调频模式即扫频
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：RFSK和Chirp之间的区别是， RFSK被限制在f1和f2之间的操作， 而Chirp操作没有f2的限制频率。

AD9854_UDCLK更新脚，每发送一个上升沿， 中断当前斜坡扫频， 将频率重置到设置的起始点频率
AD9854_FSK_BPSK_HOLD保持脚，置高时，停止扫频，输出当前频率。引脚返回低时，从停止时的频率恢复扫频。
0x1f寄存器：[7]CLR_ACC1位:当该位保持为高，更新脚上升沿可中断当前斜坡扫频，将频率重置到设置的起始点频率。
						[6]CLR_ACC2位: 当该位设置为高时， 相位累加器的输出导致DDS输出0 Hz。  要返回到以前的DDS操作， CLR ACC2必须设置为逻辑低。
**************************************************************/
void AD9854_InitChirp(void)
{
	AD9854_WR = 1; // 将读、写控制端口设为无效
	AD9854_RD = 1;
	AD9854_UDCLK = 0;
	AD9854_RST = 1; // 复位AD9854
	AD9854_RST = 0;

	AD9854_WR_Byte(0x1d, 0x00); // 开启比较器
	//	AD9854_WR_Byte(0x1d,0x10);	       //关闭比较器
	AD9854_WR_Byte(0x1e, CLK_Set); // 设置系统时钟倍频
	AD9854_WR_Byte(0x1f, 0x86);	   // 设置系统为模式3，Chirp,CLR_ACC1=1,CLR_ACC2=0
	AD9854_WR_Byte(0x20, 0x60);	   // 设置为可调节幅度，取消插值补偿

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：AD9854_SetChirp(void)
** 函数功能 ：AD9854的Chirp扫频参数设置
** 入口参数 ：Freq						起始频率	   48Bit 范围：1Hz~120000000Hz(120M Hz)
							Shape  				幅度设置. 为12 Bit,取值范围为(0~4095) ,取值越大,幅度越大
							Freq_Up				步进频率	  	 48Bit 范围：1Hz~120000000Hz(120M Hz)
							FreRate						斜率时钟控制  20Bit 范围：0~1048575
** 出口参数 ：无
** 函数说明 ：	频点间停留时间 dt_P = (FreRate+1) * (SYSCLK) ；例：FreRate=1048575，则dt_P=3.495mS
**************************************************************/
void AD9854_SetChirp(ulong Freq, uint Shape, ulong Freq_Up_Down, ulong FreRate)
{
	uchar count = 6;
	uchar Adress1, Adress2;

	Adress1 = 0x04; // 选择频率控制字地址的初值
	Adress2 = 0x10;

	Freq_convert(Freq); // 频率1转换

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress1++, FreqWord[--count]);
	}

	Freq_convert(Freq_Up_Down); // 步进频率转换

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress2++, FreqWord[--count]);
	}

	AD9854_WR_Byte(0x1a, (uchar)((FreRate >> 16) & 0x0f)); // 设置斜升速率
	AD9854_WR_Byte(0x1b, (uchar)(FreRate >> 8));
	AD9854_WR_Byte(0x1c, (uchar)FreRate);

	AD9854_WR_Byte(0x21, Shape >> 8); // 设置I通道幅度
	AD9854_WR_Byte(0x22, (uchar)(Shape & 0xff));

	AD9854_WR_Byte(0x23, Shape >> 8); // 设置Q通道幅度
	AD9854_WR_Byte(0x24, (uchar)(Shape & 0xff));

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_InitBPSK(void)
** 函数功能 ：AD9854的BPSK模式初始化
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9854_InitBPSK(void)
{
	AD9854_WR = 1; // 将读、写控制端口设为无效
	AD9854_RD = 1;
	AD9854_UDCLK = 0;
	AD9854_RST = 1; // 复位AD9854
	AD9854_RST = 0;

	AD9854_WR_Byte(0x1d, 0x00); // 开启比较器
	//	AD9854_WR_Byte(0x1d,0x10);	       //关闭比较器
	AD9854_WR_Byte(0x1e, CLK_Set); // 设置系统时钟倍频
	AD9854_WR_Byte(0x1f, 0x08);	   // 设置系统为模式4，BPSK，由外部更新
	AD9854_WR_Byte(0x20, 0x60);	   // 设置为可调节幅度，取消插值补偿

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_SetBPSK(uint Phase1,uint Phase2)
** 函数功能 ：AD9854的BPSK参数设置
** 入口参数 ：Freq   频率设置，取值范围为0~(1/2)*SYSCLK
							Shape  幅度设置. 为12 Bit,取值范围为(0~4095) ,取值越大,幅度越大
							Phase1	调制相位1
							Phase2	调制相位2
** 出口参数 ：无
** 函数说明 ：相位为14Bit，取值从0~16383对应0~360°，建议在用本函数的时候将Phase1设置为0，
							将Phase2设置为8192即180°，便于观察。
**************************************************************/
void AD9854_SetBPSK(ulong Freq, uint Shape, uint Phase1, uint Phase2)
{
	uchar count;

	uchar Adress;
	Adress = 0x04; // 选择频率控制字1地址的初值

	AD9854_WR_Byte(0x00, Phase1 >> 8); // 设置相位1
	AD9854_WR_Byte(0x01, (uchar)(Phase1 & 0xff));

	AD9854_WR_Byte(0x02, Phase2 >> 8); // 设置相位2
	AD9854_WR_Byte(0x03, (uchar)(Phase2 & 0xff));

	Freq_convert(Freq); // 频率转换

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress++, FreqWord[--count]);
	}

	AD9854_WR_Byte(0x21, Shape >> 8); // 设置I通道幅度
	AD9854_WR_Byte(0x22, (uchar)(Shape & 0xff));

	AD9854_WR_Byte(0x23, Shape >> 8); // 设置Q通道幅度
	AD9854_WR_Byte(0x24, (uchar)(Shape & 0xff));

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_InitOSK(void)
** 函数功能 ：AD9854的OSK模式初始化
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9854_InitOSK(void)
{
	AD9854_WR = 1; // 将读、写控制端口设为无效
	AD9854_RD = 1;
	AD9854_UDCLK = 0;
	AD9854_RST = 1; // 复位AD9854
	AD9854_RST = 0;

	AD9854_WR_Byte(0x1d, 0x00); // 开启比较器
	//	AD9854_WR_Byte(0x1d,0x10);	     	//关闭比较器
	AD9854_WR_Byte(0x1e, CLK_Set); // 设置系统时钟倍频
	AD9854_WR_Byte(0x1f, 0x00);	   // 设置系统为模式0，由外部更新
	AD9854_WR_Byte(0x20, 0x70);	   // 设置为可调节幅度，取消插值补偿,通断整形内部控制

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}

/************************************************************
** 函数名称 ：void AD9854_SetOSK(uchar RateShape)
** 函数功能 ：AD9854的OSK参数设置
** 入口参数 ：Freq  载波频率设置，取值范围为0~(1/2)*SYSCLK
						RateShape    OSK斜率,取值为4~255，小于4则无效
** 出口参数 ：无
** 函数说明 ：斜率时间 T = 4096 * (RateShape) * (1/SYSCLK) ；例：RateShape=50，则T≈683uS
**************************************************************/
void AD9854_SetOSK(ulong Freq, uchar RateShape)
{
	uchar count;

	uchar Adress;
	Adress = 0x04; // 选择频率控制字地址的初值

	Freq_convert(Freq); // 频率转换

	for (count = 6; count > 0;) // 写入6字节的频率控制字
	{
		AD9854_WR_Byte(Adress++, FreqWord[--count]);
	}

	AD9854_WR_Byte(0x25, RateShape); // 设置OSK斜率

	AD9854_UDCLK = 1; // 更新AD9854输出
	AD9854_UDCLK = 0;
}
