/**
 ******************************************************************************
 * @file			:bsp_ad9959.c
 * @brief			:The board support package for AD9959.
 * @version			:0.1.1
 * @author			:July
 * @date			:2022.07.01
 * @updata			:2022.07.14
 ******************************************************************************
 * @pinset		:
 *
 *				F1RC:
 *							GPIOB_PIN_3 	---> P0
 *							GPIOB_PIN_4 	---> P1
 *							GPIOB_PIN_5		---> P2
 *							GPIOB_PIN_6		---> P3
 *							GPIOB_PIN_7		---> SD0
 *							GPIOC_PIN_10	---> SD1
 *							GPIOC_PIN_11	---> SD2
 *							GPIOC_PIN_9		---> SD3
 *							GPIOC_PIN_12	---> PDC/PWR
 *							GPIOA_PIN_15	---> UPDATA
 *							GPIOC_PIN_5		---> RST
 *							GPIOC_PIN_7		---> CS
 *							GPIOC_PIN_8		---> SCK
 *
 *				F4ZG:
 *							GPIOD_PIN_9 	---> P0
 *							GPIOE_PIN_15 	---> P1
 *							GPIOE_PIN_13	---> P2
 *							GPIOE_PIN_11	---> P3
 *							GPIOE_PIN_9		---> SD0
 *							GPIOE_PIN_7		---> SD1
 *							GPIOD_PIN_0		---> SD2
 *							GPIOD_PIN_1		---> SD3
 *							GPIOD_PIN_8 	---> PDC/PWR
 *							GPIOE_PIN_12	---> UPDATA
 *							GPIOE_PIN_14	---> RST
 *							GPIOE_PIN_10	---> CS
 *							GPIOE_PIN_8 	---> SCK
 *
 *******************************************************************************
 */

#include "bsp_ad9959.h"
#include "gpio.h"

#if AD9959_USART_DRIVE
uint8_t ad9959_mode[][10] = {"POINT", "SWEEP", "FSK2", "FSK4", "AM"};
uint32_t ad9959_param = 0;
#else

uint32_t FRE_Send = 0;

uint8_t AD9959_Mode = 0;		   // 扫频或者点频
uint8_t AD9959_Wave_Show_Mode = 1; // 跟随扫频--1  不跟随扫频--0

/********************************扫频/扫相*********************************/
/*
 * 通用
 * */
uint8_t AD9959_SweepWaveFlag = 0;	 // 是否显示波形
uint32_t AD9959_SweepCount = 0;		 // extern到定时器中--ms
uint32_t AD9959_SweepCountTimes = 0; // extern到定时器中
uint32_t AD9959_SweepTime = 2;		 // 扫频间隔时间--ms

/*
 * 扫频
 * */
uint32_t AD9959_SweepMaxFre = 1000000; // 最大扫频频率--Hz
uint32_t AD9959_SweepMinFre = 1000;	   // 最小扫频频率--Hz
uint32_t AD9959_SweepStepFre = 1000;   // 扫频步进频率--Hz

/*
 * 扫相  0~16383(对应角度：0°~360°)
 * */
uint32_t AD9959_SweepMaxPha = 16383; // 最大扫描相位
uint32_t AD9959_SweepMinPha = 0;	 // 最小扫描相位
uint32_t AD9959_SweepStepPha = 450;	 // 扫相步进相位

/********************************点频*********************************/
uint32_t AD9959_FixedMaxFre = 300000; // 最大固定输出频率--Hz
uint32_t AD9959_FixedMinFre = 10000;  // 最小固定输出频率--Hz
uint32_t AD9959_FixedStepFre = 10000; // 步进固定输出频率--Hz
uint32_t AD9959_FixedAmpli = 1023;	  // 最大输出频率幅度--Hz
uint32_t AD9959_FixedNowFre = 15000;  // 此时输出频率--Hz

uint32_t AD9959_NowSinFre[5] = {0};
uint32_t AD9959_NowSinAmp[5] = {0};
uint32_t AD9959_NowSinPhr[5] = {0};

int AD9959_FixedPhase_Differ = 0;	   // 相位
int AD9959_FixedPhase_Differ_Last = 0; // 相位
uint8_t AD9959_FixedAmpli_Differ = 0;  // 幅度
int AD9959_FixedAmpli_Eror = -40;	   // 相位计算校准

uint8_t CSR_DATA0[1] = {0x10}; // 开 CH0
uint8_t CSR_DATA1[1] = {0x20}; // 开 CH1
uint8_t CSR_DATA2[1] = {0x40}; // 开 CH2
uint8_t CSR_DATA3[1] = {0x80}; // 开 CH3

double ACC_FRE_FACTOR = 8.589934592; // 频率因子8.589934592=(2^32)/500000000 其中500M=25M*20(倍频数可编程)

uint8_t FR1_DATA[3] = {0xD0, 0x00, 0x00}; // VCO gain control[23]=1系统时钟高于255Mhz; PLL[22:18]=10100,20倍频,20*25M=500MHZ; Charge pump control = 75uA
uint8_t FR2_DATA[2] = {0x00, 0x00};		  // 双方向扫描，即从起始值扫到结束值后，又从结束值扫到起始值
// uint8_t FR2_DATA[2] = {0x80,0x00};// 单方向扫描，即从起始值扫到结束值后，又从起始值扫到结束值，以此类推
uint8_t CFR_DATA[3] = {0x00, 0x03, 0x02}; // default Value = 0x000302 通道功能寄存器

uint8_t CPOW0_DATA[2] = {0x00, 0x00}; // default Value = 0x0000   @ = POW/2^14*360

uint8_t LSRR_DATA[2] = {0x00, 0x00}; // default Value = 0x----

uint8_t RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00}; // default Value = 0x--------

uint8_t FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00}; // default Value = 0x--------

uint32_t SinFre[5] = {40000, 40000, 40000, 40000};
uint32_t SinAmp[5] = {1023, 1023, 1023, 1023};
uint32_t SinPhr[5] = {0, 455, 4096 * 2 - 1, 4096 * 3 - 1};

#ifdef __BSP_STM32F1
void AD9959_Drv_Init(void)
{
	/* GPIO Ports Clock Enable */
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIOA pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIOB pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);

	/*Configure GPIOC pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 |
						  GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
#else
void AD9959_Drv_Init(void)
{
	/* GPIO Ports Clock Enable */
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitStruct.Pin = AD9959_PS1_Pin | AD9959_PS2_Pin | AD9959_PS3_Pin |
						  AD9959_SDIO0_Pin | AD9959_SDIO1_Pin | AD9959_UPDATE_Pin |
						  AD9959_CS_Pin | AD9959_SCLK_Pin | AD9959_Reset_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AD9959_PS0_Pin | AD9959_SDIO2_Pin | AD9959_PWR_Pin | AD9959_SDIO3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
#endif

// 延时
void delay1(uint32_t length)
{
	length = length * 12;
	while (length--)
		;
}
// IO口初始化
void Intserve(void)
{
	AD9959_PWR_RESET;
	AD9959_CS_SET;
	AD9959_SCLK_RESET;
	AD9959_UPDATE_RESET;
	AD9959_PS0_RESET;
	AD9959_PS1_RESET;
	AD9959_PS2_RESET;
	AD9959_PS3_RESET;
	AD9959_SDIO0_RESET;
	AD9959_SDIO1_RESET;
	AD9959_SDIO2_RESET;
	AD9959_SDIO3_RESET;
}
// AD9959复位
void IntReset(void)
{
	AD9959_Reset_RESET;
	delay1(1); // 1
	AD9959_Reset_SET;
	delay1(30); // 30
	AD9959_Reset_RESET;
}
// AD9959更新数据
void IO_Update(void)
{
	AD9959_UPDATE_RESET;
	delay1(2); // 2
	AD9959_UPDATE_SET;
	delay1(4); // 4
	AD9959_UPDATE_RESET;
}
/*--------------------------------------------
函数功能：控制器通过SPI向AD9959写数据
RegisterAddress: 寄存器地址
NumberofRegisters: 所含字节数
*RegisterData: 数据起始地址
temp: 是否更新IO寄存器
----------------------------------------------*/
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData, uint8_t temp)
{
	uint8_t ControlValue = 0;
	uint8_t ValueToWrite = 0;
	uint8_t RegisterIndex = 0;
	uint8_t i = 0;

	ControlValue = RegisterAddress;
	// 写入地址
	AD9959_SCLK_RESET;
	AD9959_CS_RESET;
	for (i = 0; i < 8; i++)
	{
		AD9959_SCLK_RESET;
		if (0x80 == (ControlValue & 0x80))
			AD9959_SDIO0_SET;
		else
			AD9959_SDIO0_RESET;
		AD9959_SCLK_SET;
		ControlValue <<= 1;
	}
	AD9959_SCLK_RESET;
	// 写入数据
	for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i = 0; i < 8; i++)
		{
			AD9959_SCLK_RESET;
			if (0x80 == (ValueToWrite & 0x80))
				AD9959_SDIO0_SET;
			else
				AD9959_SDIO0_RESET;
			AD9959_SCLK_SET;
			ValueToWrite <<= 1;
		}
		AD9959_SCLK_RESET;
	}
	if (temp != 0)
		IO_Update();
	AD9959_CS_SET;
}

/************************************************************
** 函数名称 ：void AD9959_WriteData(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData)
** 函数功能 ：使用模拟SPI向AD9959写数据
** 入口参数 ：RegisterAddress: 寄存器地址
						NumberofRegisters: 要写入的字节数
						*RegisterData: 数据起始地址
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_WriteData(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData)
{
	uint8_t ControlValue = 0;
	uint8_t ValueToWrite = 0;
	uint8_t RegisterIndex = 0;
	uint8_t i = 0;

	ControlValue = RegisterAddress;
	// 写入地址
	SCLK = 0;
	CS = 0;
	for (i = 0; i < 8; i++)
	{
		SCLK = 0;
		if (0x80 == (ControlValue & 0x80))
			SDIO0 = 1;
		else
			SDIO0 = 0;
		SCLK = 1;
		ControlValue <<= 1;
	}
	SCLK = 0;
	// 写入数据
	for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i = 0; i < 8; i++)
		{
			SCLK = 0;
			if (0x80 == (ValueToWrite & 0x80))
				SDIO0 = 1;
			else
				SDIO0 = 0;
			SCLK = 1;
			ValueToWrite <<= 1;
		}
		SCLK = 0;
	}
	CS = 1;
}

/************************************************************
** 函数名称 ：void Write_CFTW0(uint32_t fre)
** 函数功能 ：写CFTW0通道频率转换字寄存器
** 入口参数 ： Freq:	写入频率，范围0~200 000 000 Hz
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_CFTW0(uint32_t fre)
{
	uint8_t CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00}; // 中间变量
	uint32_t Temp;
	Temp = (uint32_t)fre * ACC_FRE_FACTOR;
	CFTW0_DATA[3] = (uint8_t)Temp;
	CFTW0_DATA[2] = (uint8_t)(Temp >> 8);
	CFTW0_DATA[1] = (uint8_t)(Temp >> 16);
	CFTW0_DATA[0] = (uint8_t)(Temp >> 24);
	AD9959_WriteData(CFTW0_ADD, 4, CFTW0_DATA); // CTW0 address 0x04
}

/************************************************************
** 函数名称 ：void Write_ACR(uint16_t Ampli)
** 函数功能 ：写ACR通道幅度转换字寄存器
** 入口参数 ：Ampli:    输出幅度,范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_ACR(uint16_t Ampli)
{
	uint32_t A_temp = 0;
	uint8_t ACR_DATA[3] = {0x00, 0x00, 0x00}; // default Value = 0x--0000 Rest = 18.91/Iout
	A_temp = Ampli | 0x1000;

	ACR_DATA[1] = (uint8_t)(A_temp >> 8);	// 高位数据
	ACR_DATA[2] = (uint8_t)A_temp;			// 低位数据
	AD9959_WriteData(ACR_ADD, 3, ACR_DATA); // ACR address 0x06.CHn设定幅度
}

/************************************************************
** 函数名称 ：void Write_CPOW0(uint16_t Phase)
** 函数功能 ：写CPOW0通道相位转换字寄存器
** 入口参数 ：Phase:		输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_CPOW0(uint16_t Phase)
{
	uint8_t CPOW0_data[2] = {0x00, 0x00};
	CPOW0_data[1] = (uint8_t)Phase;
	CPOW0_data[0] = (uint8_t)(Phase >> 8);

	AD9959_WriteData(CPOW0_ADD, 2, CPOW0_data); // CPOW0 address 0x05.CHn设定相位
}

/************************************************************
** 函数名称 ：void Write_LSRR(uint8_t rsrr,uint8_t fsrr)
** 函数功能 ：写LSRR线性扫描斜率寄存器
** 入口参数 ：	rsrr:	上升斜率,范围：0~255
							fsrr:	下降斜率,范围：0~255
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_LSRR(uint8_t rsrr, uint8_t fsrr)
{
	uint8_t LSRR_data[2] = {0x00, 0x00};

	LSRR_data[1] = rsrr;
	LSRR_data[0] = fsrr; // 高8位下降斜率

	AD9959_WriteData(LSRR_ADD, 2, LSRR_data);
}

/************************************************************
** 函数名称 ：void Write_RDW(uint32_t r_delta)
** 函数功能 ：写RDW上升增量寄存器
** 入口参数 ：r_delta:上升增量,0-4294967295
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_RDW(uint32_t r_delta)
{
	uint8_t RDW_data[4] = {0x00, 0x00, 0x00, 0x00}; // 中间变量

	RDW_data[3] = (uint8_t)r_delta;
	RDW_data[2] = (uint8_t)(r_delta >> 8);
	RDW_data[1] = (uint8_t)(r_delta >> 16);
	RDW_data[0] = (uint8_t)(r_delta >> 24);
	AD9959_WriteData(RDW_ADD, 4, RDW_data);
}

/************************************************************
** 函数名称 ：void Write_FDW(uint32_t f_delta)
** 函数功能 ：写FDW下降增量寄存器
** 入口参数 ：f_delta:下降增量,0-4294967295
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_FDW(uint32_t f_delta)
{
	uint8_t FDW_data[4] = {0x00, 0x00, 0x00, 0x00}; // 中间变量

	FDW_data[3] = (uint8_t)f_delta;
	FDW_data[2] = (uint8_t)(f_delta >> 8);
	FDW_data[1] = (uint8_t)(f_delta >> 16);
	FDW_data[0] = (uint8_t)(f_delta >> 24);
	AD9959_WriteData(FDW_ADD, 4, FDW_data);
}

/************************************************************
** 函数名称 ：void Write_Profile_Fre(uint8_t profile,uint32_t data)
** 函数功能 ：写Profile寄存器
** 入口参数 ：profile:	profile号(0~14)
			  data:	写入频率，范围0~200 000 000 Hz
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_Profile_Fre(uint8_t profile, uint32_t data)
{
	uint8_t profileAddr;
	uint8_t Profile_data[4] = {0x00, 0x00, 0x00, 0x00}; // 中间变量
	uint32_t Temp;
	Temp = (uint32_t)data * ACC_FRE_FACTOR; // 将输入频率因子分为四个字节  4.294967296=(2^32)/500000000
	Profile_data[3] = (uint8_t)Temp;
	Profile_data[2] = (uint8_t)(Temp >> 8);
	Profile_data[1] = (uint8_t)(Temp >> 16);
	Profile_data[0] = (uint8_t)(Temp >> 24);
	profileAddr = PROFILE_ADDR_BASE + profile;

	AD9959_WriteData(profileAddr, 4, Profile_data);
}

/************************************************************
** 函数名称 ：void Write_Profile_Ampli(uint8_t profile,uint16_t data)
** 函数功能 ：写Profile寄存器
** 入口参数 ：profile:	profile号(0~14)
			  data:	 写入幅度,范围0~1023，
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_Profile_Ampli(uint8_t profile, uint16_t data)
{
	uint8_t profileAddr;
	uint8_t Profile_data[4] = {0x00, 0x00, 0x00, 0x00}; // 中间变量

	// 为幅度调制时幅度数据为[31:22]位
	Profile_data[1] = (uint8_t)(data << 6); //[23:22]
	Profile_data[0] = (uint8_t)(data >> 2); //[31:24]

	profileAddr = PROFILE_ADDR_BASE + profile;

	AD9959_WriteData(profileAddr, 4, Profile_data); // 写入32位数据
}

/************************************************************
** 函数名称 ：void Write_Profile_Phase(uint8_t profile,uint16_t data)
** 函数功能 ：写Profile寄存器
** 入口参数 ：profile:	profile号(0~14)
			  data:	 写入相位,范围：0~16383
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_Profile_Phase(uint8_t profile, uint16_t data)
{
	uint8_t profileAddr;
	uint8_t Profile_data[4] = {0x00, 0x00, 0x00, 0x00}; // 中间变量

	// 为相位调制时相位数据为[31:18]位
	Profile_data[1] = (uint8_t)(data << 2); //[23:18]
	Profile_data[0] = (uint8_t)(data >> 6); //[31:24]

	profileAddr = PROFILE_ADDR_BASE + profile;

	AD9959_WriteData(profileAddr, 4, Profile_data); // 写入32位数据
}

/************************************************************
** 函数名称 ：void AD9959_Modulation_Init(uint8_t Channel,uint8_t Modulation,uint8_t Sweep_en,uint8_t Nlevel)
** 函数功能 ：设置各个通道的调制模式。
** 入口参数 ： Channel:  	输出通道 CH0-CH3
							Modulation:	调制模式DISABLE_Mod，ASK，FSK，PSK
							Sweep_en:		线性扫描模式 SWEEP_ENABLE启用、SWEEP_DISABLE不启用；启用时Nlevel只能是LEVEL_MOD_2
							Nlevel：		调制电平选择 LEVEL_MOD_2、4、8、16
** 出口参数 ：无
** 函数说明 ：如将调制电平设置为2电平调制时，对应的P0-P3引脚上的高低电平分别控制CH0-CH3通道(如果对应通道开启的话)
							如将调制电平设置为4电平调制时，对应的P0，P1和P2,P3引脚上的高低电平分别控制CH0-CH1通道(如果对应通道开启的话)
							由于AD9959只有P0-P3,4个用于调制控制的引脚，因此输出在4电平调制时，只能有2个通道同时设置为调制输出；
							8电平和16电平调制时，只能有1个通道同时设置为调制输出。请适当设置几电平调制以满足应用需求。

**注意！！！：设置成4电平调制时，输出通道只能选择CH0-1
							设置成8,16电平调制时，输出通道只能选择CH0
							本函数未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
**************************************************************/
void AD9959_Modulation_Init(uint8_t Channel, uint8_t Modulation, uint8_t Sweep_en, uint8_t Nlevel)
{
	uint8_t i = 0;
	uint8_t CHANNEL[1] = {0x00};
	uint8_t FR1_data[3];
	uint8_t FR2_data[2];
	uint8_t CFR_data[3];
	for (i = 0; i < 3; i++) // 设置默认值
	{
		FR1_data[i] = FR1_DATA[i];
		CFR_data[i] = CFR_DATA[i];
	}
	FR2_data[0] = FR2_DATA[0];
	FR2_data[1] = FR2_DATA[1];

	CHANNEL[0] = Channel;
	AD9959_WriteData(CSR_ADD, 1, CHANNEL); // 控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn

	FR1_data[1] = Nlevel;
	CFR_data[0] = Modulation;
	CFR_data[1] |= Sweep_en;
	CFR_data[2] = 0x00;

	if (Channel != 0) // 有通道被使能
	{
		AD9959_WriteData(FR1_ADD, 3, FR1_data); // 写功能寄存器1
		AD9959_WriteData(FR2_ADD, 2, FR2_data); // 写功能寄存器1
		AD9959_WriteData(CFR_ADD, 3, CFR_data); // 写通道功能寄存器
	}
}

/************************************************************
** 函数名称 ：void AD9959_SetFSK(uint8_t Channel, uint32_t *data,uint16_t Phase)
** 函数功能 ：设置FSK调制的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							*data:	调整频率数据的起始地址
							Phase:	输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：FSK时信号幅度默认为最大
**************************************************************/
void AD9959_SetFSK(uint8_t Channel, uint32_t *data, uint16_t Phase)
{
	uint8_t i = 0;
	uint8_t CHANNEL[1] = {0x00};

	CHANNEL[0] = Channel;
	AD9959_WriteData(CSR_ADD, 1, CHANNEL); // 控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	Write_CPOW0(Phase);					   // 设置相位

	Write_CFTW0(data[0]);
	for (i = 0; i < 15; i++)
		Write_Profile_Fre(i, data[i + 1]);
}

/************************************************************
** 函数名称 ：void AD9959_SetASK(uint8_t Channel, uint32_t *data,uint32_t fre,uint16_t Phase)
** 函数功能 ：设置ASK调制的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							*data: 调整幅度数据的起始地址
							Freq:		输出频率，范围0~200 000 000 Hz
							Phase:	输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_SetASK(uint8_t Channel, uint16_t *data, uint32_t fre, uint16_t Phase)
{
	uint8_t i = 0;
	uint8_t CHANNEL[1] = {0x00};

	CHANNEL[0] = Channel;
	AD9959_WriteData(CSR_ADD, 1, CHANNEL); // 控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	Write_CFTW0(fre);					   // 设置频率
	Write_CPOW0(Phase);					   // 设置相位

	Write_ACR(data[0]);
	for (i = 0; i < 15; i++)
		Write_Profile_Ampli(i, data[i + 1]);
}

/************************************************************
** 函数名称 ：void AD9959_SetPSK(uint8_t Channel, uint16_t *data,uint32_t fre,uint16_t Phase)
** 函数功能 ：设置PSK调制的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							*data:	调整相位数据的起始地址
							Freq:		输出频率，范围0~200 000 000 Hz
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_SetPSK(uint8_t Channel, uint16_t *data, uint32_t Freq)
{
	uint8_t i = 0;
	uint8_t CHANNEL[1] = {0x00};

	CHANNEL[0] = Channel;
	AD9959_WriteData(CSR_ADD, 1, CHANNEL); // 控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	Write_CFTW0(Freq);

	Write_CPOW0(data[0]);
	for (i = 0; i < 15; i++)
		Write_Profile_Phase(i, data[i + 1]);
}

/************************************************************
** 函数名称 ：void AD9959_SetFre_Sweep(uint8_t Channel, uint32_t s_data,uint32_t e_data,uint8_t fsrr,uint8_t rsrr,uint32_t r_delta,uint32_t f_delta,uint16_t Phase)
** 函数功能 ：设置线性扫频的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							s_data:	起始频率，范围0~200 000 000 Hz
							e_data:	结束频率，范围0~200 000 000 Hz
							r_delta:上升步长频率,0~200 000 000Hz
							f_delta:下降步长频率,0~200 000 000Hz

							rsrr:		上升斜率,范围：1~255，系统时钟为500Mhz时一个控制字约为8ns
							fsrr:		下降斜率,范围：1~255
							Ampli:	输出幅度,范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
							Phase:	输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：频点与频点间停留时间 dT = Xsrr*8 单位ns，扫描点数=(起始-结束)/步长
							扫频总时间=总扫描频点数*dT
**************************************************************/
void AD9959_SetFre_Sweep(uint8_t Channel, uint32_t s_data, uint32_t e_data, uint32_t r_delta, uint32_t f_delta, uint8_t rsrr, uint8_t fsrr, uint16_t Ampli, uint16_t Phase)
{
	uint8_t CHANNEL[1] = {0x00};
	uint32_t Fer_data = 0;

	CHANNEL[0] = Channel;
	AD9959_WriteData(CSR_ADD, 1, CHANNEL); // 控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn

	Write_CPOW0(Phase); // 设置相位
	Write_ACR(Ampli);	// 幅度设置

	Write_LSRR(rsrr, fsrr); // 斜率

	Fer_data = (uint32_t)r_delta * ACC_FRE_FACTOR; // 频率转换成控制字
	Write_RDW(Fer_data);						   // 上升步长

	Fer_data = (uint32_t)f_delta * ACC_FRE_FACTOR;
	Write_FDW(Fer_data); // 下降步长

	Write_CFTW0(s_data);		  // 起始频率
	Write_Profile_Fre(0, e_data); // 结束频率
}

/************************************************************
** 函数名称 ：void AD9959_SetAmp_Sweep(uint8_t Channel, uint32_t s_Ampli,uint16_t e_Ampli,uint32_t r_delta,uint32_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Phase)
** 函数功能 ：设置线性扫幅的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							s_Ampli:	起始幅度，控制值0~1023对应输出幅度0~500mVpp左右
							e_Ampli:	结束幅度，

							r_delta:	上升步长幅度,0~1023
							f_delta:	下降步长幅度,0~1023

							rsrr:			上升斜率,范围：1~255，系统时钟为500Mhz时一个控制字约为8ns
							fsrr:			下降斜率,范围：1~255

							fre:			输出频率，范围0~200 000 000 Hz
							Phase:		输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：幅点与幅点间停留时间 dT = Xsrr*8 单位ns；扫描点数=(起始-结束)/步长
							扫幅总时间=总扫描幅点数*dT
**************************************************************/
void AD9959_SetAmp_Sweep(uint8_t Channel, uint32_t s_Ampli, uint16_t e_Ampli, uint32_t r_delta, uint32_t f_delta, uint8_t rsrr, uint8_t fsrr, uint32_t fre, uint16_t Phase)
{
	uint8_t CHANNEL[1] = {0x00};
	uint8_t ACR_data[3] = {0x00, 0x00, 0x00};

	CHANNEL[0] = Channel;
	AD9959_WriteData(CSR_ADD, 1, CHANNEL); // 控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn

	Write_CFTW0(fre);	// 幅度频率
	Write_CPOW0(Phase); // 设置相位

	Write_LSRR(rsrr, fsrr); // 斜率

	Write_RDW(r_delta << 22); // 上升步长

	Write_FDW(f_delta << 22); // 下降步长

	ACR_data[1] = (uint8_t)(s_Ampli >> 8);	// 高位数据
	ACR_data[2] = (uint8_t)s_Ampli;			// 低位数据
	AD9959_WriteData(ACR_ADD, 3, ACR_data); // ACR 设定起始幅度

	Write_Profile_Ampli(0, e_Ampli); // 结束幅度
}

/************************************************************
** 函数名称 ：void AD9959_SetPhase_Sweep(uint8_t Channel, uint16_t s_data,uint16_t e_data,uint16_t r_delta,uint16_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Ampli)
** 函数功能 ：设置线性扫相的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							s_data:	起始相位，范围：0~16383(对应角度：0°~360°)
							e_data:	结束相位，
							r_delta:上升步长,范围：0~16383(对应角度：0°~360°)
							f_delta:下降步长,

							rsrr:		上升斜率,范围：1~255，系统时钟为500Mhz时一个控制字约为8ns
							fsrr:		下降斜率,范围：1~255
							fre:		输出频率，范围0~200 000 000 Hz
							Ampli:	输出幅度,范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
** 出口参数 ：无
** 函数说明 ：频点与频点间停留时间 dT = Xsrr*8 单位ns；扫描点数=(起始-结束)/步长
							扫频总时间=总扫描频点数*dT
**************************************************************/
void AD9959_SetPhase_Sweep(uint8_t Channel, uint16_t s_data, uint16_t e_data, uint16_t r_delta, uint16_t f_delta, uint8_t rsrr, uint8_t fsrr, uint32_t fre, uint16_t Ampli)
{
	uint8_t CHANNEL[1] = {0x00};

	CHANNEL[0] = Channel;
	AD9959_WriteData(CSR_ADD, 1, CHANNEL); // 控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn

	Write_CFTW0(fre); // 幅度频率
	Write_ACR(Ampli); // 幅度设置

	Write_LSRR(rsrr, fsrr); // 斜率

	Write_RDW(r_delta << 18); // 上升步长

	Write_FDW(f_delta << 18); // 下降步长

	Write_CPOW0(s_data);			// 起始相位
	Write_Profile_Phase(0, e_data); // 结束相位
}

/*---------------------------------------
函数功能：设置通道输出频率
Channel:  输出通道
Freq:     输出频率
---------------------------------------*/
void AD9959_WriteFreq(uint8_t Channel, uint32_t Freq)
{
	uint8_t CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00}; // 中间变量
	uint32_t Temp;
	Temp = (uint32_t)Freq * 8.589934592; // 将输入频率因子分为四个字节  8.589934592=(2^32)/500000000 其中500M=25M*20(倍频数可编程)
	CFTW0_DATA[3] = (uint8_t)Temp;
	CFTW0_DATA[2] = (uint8_t)(Temp >> 8);
	CFTW0_DATA[1] = (uint8_t)(Temp >> 16);
	CFTW0_DATA[0] = (uint8_t)(Temp >> 24);
	if (Channel == 0)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1);	   // 控制寄存器写入CH0通道
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); // CTW0 address 0x04.输出CH0设定频率
	}
	else if (Channel == 1)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1);	   // 控制寄存器写入CH1通道
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); // CTW0 address 0x04.输出CH1设定频率
	}
	else if (Channel == 2)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1);	   // 控制寄存器写入CH2通道
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); // CTW0 address 0x04.输出CH2设定频率
	}
	else if (Channel == 3)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1);	   // 控制寄存器写入CH3通道
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); // CTW0 address 0x04.输出CH3设定频率
	}
}
/*---------------------------------------
函数功能：设置通道输出幅度
Channel:  输出通道
Ampli:    输出幅度
---------------------------------------*/
void AD9959_WriteAmpl(uint8_t Channel, uint16_t Ampli)
{
	uint16_t A_temp;						  //=0x23ff;
	uint8_t ACR_DATA[3] = {0x00, 0x00, 0x00}; // default Value = 0x--0000 Rest = 18.91/Iout
	A_temp = Ampli | 0x1000;
	ACR_DATA[2] = (uint8_t)A_temp;		  // 低位数据
	ACR_DATA[1] = (uint8_t)(A_temp >> 8); // 高位数据

	if (Channel == 0)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1);
		WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
	}

	else if (Channel == 1)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1);
		WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
	}

	else if (Channel == 2)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1);
		WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
	}

	else if (Channel == 3)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1);
		WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
	}
}
/*---------------------------------------
函数功能：设置通道输出相位
Channel:  输出通道
Phase:    输出相位,范围：0~16383(对应角度：0°~360°)
---------------------------------------*/
void AD9959_WritePhase(uint8_t Channel, uint16_t Phase)
{
	uint16_t P_temp = 0;
	P_temp = (uint16_t)Phase;
	CPOW0_DATA[1] = (uint8_t)P_temp;
	CPOW0_DATA[0] = (uint8_t)(P_temp >> 8);
	if (Channel == 0)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1);
		WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
	}

	else if (Channel == 1)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1);
		WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
	}

	else if (Channel == 2)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1);
		WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
	}

	else if (Channel == 3)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1);
		WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
	}
}

// AD9959初始化
void AD9959_Init(void)
{
	uint8_t FR1_DATA[3] = {0xD0, 0x00, 0x00}; // 20倍频 Charge pump control = 75uA FR1<23> -- VCO gain control =0时 system clock below 160 MHz;

	AD9959_Drv_Init();

	Intserve(); // IO口初始化
	IntReset(); // AD9959复位

	WriteData_AD9959(FR1_ADD, 3, FR1_DATA, 1); // 写功能寄存器1
	WriteData_AD9959(FR2_ADD, 2, FR2_DATA, 1);

#if 0
	WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
	WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
	WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
	WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);
	WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);
#endif

	// 写入初始频率
	AD9959_WriteFreq(3, SinFre[3]);
	AD9959_WriteFreq(0, SinFre[0]);
	AD9959_WriteFreq(1, SinFre[1]);
	AD9959_WriteFreq(2, SinFre[2]);

	AD9959_WritePhase(3, SinPhr[3]);
	AD9959_WritePhase(0, SinPhr[0]);
	AD9959_WritePhase(1, SinPhr[1]);
	AD9959_WritePhase(2, SinPhr[2]);

	AD9959_WriteAmpl(3, SinAmp[3]);
	AD9959_WriteAmpl(0, SinAmp[0]);
	AD9959_WriteAmpl(1, SinAmp[1]);
	AD9959_WriteAmpl(2, SinAmp[2]);
}
#endif
