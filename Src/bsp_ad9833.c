/**
 * @file bsp_ad9833.c
 * @author merchant
 * @brief AD9833 module driver.
 * @version 0.1
 * @date 2023.07.16
 * @pinset
 * 				-- SIGL 
 *				GPIOE_PIN_0 	---> FSYNC
 *				GPIOE_PIN_1 	---> SCK
 *				GPIOE_PIN_2 	---> DAT
 *				GPIOE_PIN_3   	---> CS
 *
 *				-- MULT
 *				GPIOB_PIN_15 	---> FSYNC1
 *				GPIOB_PIN_14 	---> SCK1
 *				GPIOB_PIN_13 	---> DAT1
 *				GPIOB_PIN_12	---> CS1n
 *
 *				GPIOC_PIN_0		---> FSYNC2
 *				GPIOC_PIN_1		---> SCK2
 *				GPIOC_PIN_2		---> DAT2
 *				GPIOC_PIN_3		---> CS2
 *
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "bsp_ad9833.h"
#include "gpio.h"
#if 0
#include "spi.h"
#endif

/**
 * @brief AD9833 延时
 */
static void AD9833_Delay(void)
{
	uint16_t i;
	for (i = 0; i < 30; i++)
		;
}
static void AD9833_Delay2(void)
{
	uint16_t i;
	for (i = 0; i < 18; i++)
		;
}
static void AD9833_Delay3(void)
{
	uint16_t i;
	for (i = 0; i < 8; i++)
		;
}

#if AD9833_DEMULT

/**
 * @brief AD9833相关引脚初始化
 *
 */
void AD9833_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOE_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/**
 * @brief 向SPI总线发送16个bit数据
 *
 * @param TxData 数据
 */
void AD9833_Write(unsigned int TxData)
{
	unsigned char i;
	/*
	uint8_t SpiTxData[2] = {0};

	SpiTxData[0] = (uint8_t)((TxData & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(TxData &  0x00ff);
	*/

	SCK(_1);

	FSY(_1);
	AD9833_Delay2();
	FSY(_0);
	AD9833_Delay2();
#if defined(SofeSpi)

	for (i = 0; i < 16; i++)
	{
		if (TxData & 0x8000)
			DAT(_1);
		else
			DAT(_0);

		AD9833_Delay2();
		SCK(_0);
		AD9833_Delay2();
		SCK(_1);

		TxData <<= 1;
	}

#endif
#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
#endif

	FSY(_1);
	AD9833_Delay2();
}

/**
 * @brief 改变输出信号幅度值
 *
 * @param amp 幅度值  0- 255
 */
void AD9833_AmpSet(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;

	temp = 0x1100 | amp;

	/*
	uint8_t SpiTxData[2] = {0};

	SpiTxData[0] = (uint8_t)((temp & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(temp &  0x00ff);
	*/

	CS(_0);
	AD9833_Delay2();

#if defined(SofeSpi)
	for (i = 0; i < 16; i++)
	{
		SCK(_0);
		AD9833_Delay2();
		if (temp & 0x8000)
		{
			DAT(_1);
			AD9833_Delay2();
		}
		else
		{
			DAT(_0);
			AD9833_Delay2();
		}
		temp <<= 1;
		SCK(_1);
		AD9833_Delay();
	}
#endif

#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
#endif

	CS(_1);
	AD9833_Delay2();
}

/**
 * @brief 向SPI总线发送16个bit数据
 *
 * @param Freq 频率值, 0.1 hz - 12Mhz
 * @param Freq_SFR 0 或 1
 * @param WaveMode TRI_WAVE(三角波),SIN_WAVE(正弦波),SQU_WAVE(方波)
 * @param Phase 波形的初相位
 */
void AD9833_WaveSeting(double Freq, unsigned int Freq_SFR, unsigned int WaveMode, unsigned int Phase)
{

	int frequence_LSB, frequence_MSB, Phs_data;
	double frequence_mid, frequence_DATA;
	long int frequence_hex;

	/*********************************计算频率的16进制值***********************************/

	frequence_mid = 268435456 / 25; // 适合25M晶振
									// 如果时钟频率不为25MHZ，修改该处的频率值，单位MHz ，AD9833最大支持25MHz

	frequence_DATA = Freq;
	frequence_DATA = frequence_DATA / 1000000;
	frequence_DATA = frequence_DATA * frequence_mid;
	frequence_hex = frequence_DATA;			// 这个frequence_hex的值是32位的一个很大的数字，需要拆分成两个14位进行处理；
	frequence_LSB = frequence_hex;			// frequence_hex低16位送给frequence_LSB
	frequence_LSB = frequence_LSB & 0x3fff; // 去除最高两位，16位数换去掉高位后变成了14位
	frequence_MSB = frequence_hex >> 14;	// frequence_hex高16位送给frequence_HSB
	frequence_MSB = frequence_MSB & 0x3fff; // 去除最高两位，16位数换去掉高位后变成了14位

	Phs_data = Phase | 0xC000; // 相位值
	AD9833_Write(0x0100);	   // 复位AD9833,即RESET位为1
	AD9833_Write(0x2100);	   // 选择数据一次写入，B28位和RESET位为1

	if (Freq_SFR == 0) // 把数据设置到设置频率寄存器0
	{
		frequence_LSB = frequence_LSB | 0x4000;
		frequence_MSB = frequence_MSB | 0x4000;
		// 使用频率寄存器0输出波形
		AD9833_Write(frequence_LSB); // L14，选择频率寄存器0的低14位数据输入
		AD9833_Write(frequence_MSB); // H14 频率寄存器的高14位数据输入
		AD9833_Write(Phs_data);		 // 设置相位
									 // AD9833_Write(0x2000); 			//设置FSELECT位为0，芯片进入工作状态,频率寄存器0输出波形
	}
	if (Freq_SFR == 1) // 把数据设置到设置频率寄存器1
	{
		frequence_LSB = frequence_LSB | 0x8000;
		frequence_MSB = frequence_MSB | 0x8000;
		// 使用频率寄存器1输出波形
		AD9833_Write(frequence_LSB); // L14，选择频率寄存器1的低14位输入
		AD9833_Write(frequence_MSB); // H14 频率寄存器1为
		AD9833_Write(Phs_data);		 // 设置相位
									 // AD9833_Write(0x2800); 			//设置FSELECT位为0，设置FSELECT位为1，即使用频率寄存器1的值，芯片进入工作状态,频率寄存器1输出波形
	}

	if (WaveMode == TRI_WAVE) // 输出三角波波形
		AD9833_Write(0x2002);
	if (WaveMode == SQU_WAVE) // 输出方波波形
		AD9833_Write(0x2028);
	if (WaveMode == SIN_WAVE) // 输出正弦波形
		AD9833_Write(0x2000);
}

#if 1
/**
 * @brief SPI发送单字节
 *
 * @param xtemp 发送的字节
 */
void MCP41010_SPI_SendOneByte(uint8_t xtemp)
{
	uint8_t xnum = 0;

	SCK(_0);
	AD9833_Delay3();
	/* DP_SCK发送8个脉冲，在上升沿时送入数据 */
	for (xnum = 0; xnum < 8; xnum++)
	{
		// DP_SI写数据
		// DP_SI = (xtemp & 0x80) >>7;
		if (xtemp & 0x80)
		{
			DAT(_1);
		}
		else
			DAT(_0);
		xtemp = xtemp << 1;

		// 延时待数据稳定后，在DP_SCK产生上升沿脉冲，数据送入MCP41010
		AD9833_Delay3();
		SCK(_1);

		// DP_SCK产生下降沿脉冲
		AD9833_Delay3();
		SCK(_0);
	}
}

/**
 * @brief 设置MCP41010的滑动变阻器抽头位置
 *
 * @param xRes 电阻编码值
 * @attention 00时，W抽头处于抽头B的位置，阻值等于抽头电阻，典型值为52Ω;
 *			  FF时，W抽头处于抽头A的位置，阻值等为9985.94Ω，抽头始终不会与A端相连;
 *			  对于10KΩ的器件来说，LSB为39.0625Ω;
 *			  每次芯片上电后，抽头寄存器的值为0x80;
 */
void MCP41010_SetRes(uint8_t xRes)
{
	CS(_1);
	SCK(_1);
	AD9833_Delay2();
	// MCP41010芯片使能
	CS(_0);
	AD9833_Delay2();
	// 发送第一字节，控制方式位
	// 0x13。写数据，对电位器0，1操作
	MCP41010_SPI_SendOneByte(0x13);

	// 发送第二字节，滑动变阻器抽头位
	MCP41010_SPI_SendOneByte(xRes);

	// MCP41010芯片失能
	CS(_1);
}

#endif

#if 0
uint8_t	CMD_NONE = 0, CMD_WRITE = 1, CMD_SHUT_DOWN = 2;

void bsp_mcp41_init(uint8_t pot_value)
{
	bsp_mcp41_set_value(1, pot_value);
#if 0
	spi_driver_start();
	spi_driver_tx();
	spi_driver_rx();
	spi_driver_stop();
#endif
}

void bsp_mcp41_set_value(uint8_t pot_id, uint8_t pot_value)
{
	uint8_t data[2] = {0};

	/* write command & pot id */
	data[0] = (CMD_WRITE << 4) | pot_id;

	/* insert pot value */
	data[1] = pot_value;

	CS_0();//spi_driver_start();
	HAL_SPI_Transmit(&hspi1, data, 2, 1000);//spi_driver_tx_dma(data, 2);
	CS_1();//spi_driver_stop();
}

void bsp_AD9833_IO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}
void bsp_AD9833_SPI_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		__HAL_RCC_SPI1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 }
#endif
#else
/*
**********************************************************************************
*	函 数 名: AD9833_MULT_Write
*	功能说明: 向SPI总线发送16个bit数据
*	形    参: TxData : 数据
*	返 回 值: 无
***********************************************************************************
*/
void AD9833_MULT_Write1(unsigned int TxData)
{
	unsigned char i;
	/*
	uint8_t SpiTxData[2] = {0};

	SpiTxData[0] = (uint8_t)((TxData & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(TxData &  0x00ff);
	*/

	SCK1(_1);

	FSY1(_1);
	AD9833_Delay2();
	FSY1(_0);
	AD9833_Delay2();
#if defined(SofeSpi)

	for (i = 0; i < 16; i++)
	{
		if (TxData & 0x8000)
			DAT1(_1);
		else
			DAT1(_0);

		AD9833_Delay2();
		SCK1(_0);
		AD9833_Delay2();
		SCK1(_1);

		TxData <<= 1;
	}

#endif
#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
#endif

	FSY1(_1);
	AD9833_Delay2();
}
void AD9833_MULT_Write2(unsigned int TxData)
{
	unsigned char i;
	/*
	uint8_t SpiTxData[2] = {0};

	SpiTxData[0] = (uint8_t)((TxData & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(TxData &  0x00ff);
	*/

	SCK2(_1);

	FSY2(_1);
	AD9833_Delay2();
	FSY2(_0);
	AD9833_Delay2();
#if defined(SofeSpi)

	for (i = 0; i < 16; i++)
	{
		if (TxData & 0x8000)
			DAT2(_1);
		else
			DAT2(_0);

		AD9833_Delay2();
		SCK2(_0);
		AD9833_Delay2();
		SCK2(_1);

		TxData <<= 1;
	}

#endif
#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
#endif

	FSY2(_1);
	AD9833_Delay2();
}
/*
***********************************************************************************
*	函 数 名: AD9833_MULT_AmpSet
*	功能说明: 改变输出信号幅度值
*	形    参: 1.amp ：幅度值  0- 255
*	返 回 值: 无
***********************************************************************************
*/

void AD9833_MULT_AmpSet1(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;

	temp = 0x1100 | amp;

	/*
	uint8_t SpiTxData[2] = {0};

	SpiTxData[0] = (uint8_t)((temp & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(temp &  0x00ff);
	*/

	CS1(_0);
	AD9833_Delay2();

#if defined(SofeSpi)
	for (i = 0; i < 16; i++)
	{
		SCK1(_0);
		AD9833_Delay2();
		if (temp & 0x8000)
		{
			DAT1(_1);
			AD9833_Delay2();
		}
		else
		{
			DAT1(_0);
			AD9833_Delay2();
		}
		temp <<= 1;
		SCK1(_1);
		AD9833_Delay();
	}
#endif

#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
#endif

	CS1(_1);
	AD9833_Delay2();
}
void AD9833_MULT_AmpSet2(unsigned char amp)
{
	unsigned char i;
	unsigned int temp;

	temp = 0x1100 | amp;

	/*
	uint8_t SpiTxData[2] = {0};

	SpiTxData[0] = (uint8_t)((temp & (0xff00)) >> 8);
	SpiTxData[1] = (uint8_t)(temp &  0x00ff);
	*/

	CS2(_0);
	AD9833_Delay2();

#if defined(SofeSpi)
	for (i = 0; i < 16; i++)
	{
		SCK2(_0);
		AD9833_Delay2();
		if (temp & 0x8000)
		{
			DAT2(_1);
			AD9833_Delay2();
		}
		else
		{
			DAT2(_0);
			AD9833_Delay2();
		}
		temp <<= 1;
		SCK2(_1);
		AD9833_Delay();
	}
#endif

#if defined(HardSpi)
	HAL_SPI_Transmit(&hspi2, SpiTxData, 2, 1000);
#endif

	CS2(_1);
	AD9833_Delay2();
}

/*
***********************************************************************************
*	函 数 名: AD9833_MULT_WaveSeting
*	功能说明: 向SPI总线发送16个bit数据
*	形    参:	1.Freq: 频率值, 0.1 hz - 12Mhz
				2.Freq_SFR: 0 或 1
				3.WaveMode: TRI_WAVE(三角波),SIN_WAVE(正弦波),SQU_WAVE(方波)
				4.Phase : 波形的初相位
*	返 回 值: 无
***********************************************************************************
*/
void AD9833_MULT_WaveSeting1(double Freq, unsigned int Freq_SFR, unsigned int WaveMode, unsigned int Phase)
{

	int frequence_LSB, frequence_MSB, Phs_data;
	double frequence_mid, frequence_DATA;
	long int frequence_hex;

	/*********************************计算频率的16进制值***********************************/

	frequence_mid = 268435456 / 25; // 适合25M晶振
									// 如果时钟频率不为25MHZ，修改该处的频率值，单位MHz ，AD9833最大支持25MHz

	frequence_DATA = Freq;
	frequence_DATA = frequence_DATA / 1000000;
	frequence_DATA = frequence_DATA * frequence_mid;
	frequence_hex = frequence_DATA;			// 这个frequence_hex的值是32位的一个很大的数字，需要拆分成两个14位进行处理；
	frequence_LSB = frequence_hex;			// frequence_hex低16位送给frequence_LSB
	frequence_LSB = frequence_LSB & 0x3fff; // 去除最高两位，16位数换去掉高位后变成了14位
	frequence_MSB = frequence_hex >> 14;	// frequence_hex高16位送给frequence_HSB
	frequence_MSB = frequence_MSB & 0x3fff; // 去除最高两位，16位数换去掉高位后变成了14位

	Phs_data = Phase | 0xC000;	// 相位值
	AD9833_MULT_Write1(0x0100); // 复位AD9833,即RESET位为1
	AD9833_MULT_Write1(0x2100); // 选择数据一次写入，B28位和RESET位为1

	if (Freq_SFR == 0) // 把数据设置到设置频率寄存器0
	{
		frequence_LSB = frequence_LSB | 0x4000;
		frequence_MSB = frequence_MSB | 0x4000;
		// 使用频率寄存器0输出波形
		AD9833_MULT_Write1(frequence_LSB); // L14，选择频率寄存器0的低14位数据输入
		AD9833_MULT_Write1(frequence_MSB); // H14 频率寄存器的高14位数据输入
		AD9833_MULT_Write1(Phs_data);	   // 设置相位
										   // AD9833_MULT_Write1(0x2000); 			//设置FSELECT位为0，芯片进入工作状态,频率寄存器0输出波形
	}
	if (Freq_SFR == 1) // 把数据设置到设置频率寄存器1
	{
		frequence_LSB = frequence_LSB | 0x8000;
		frequence_MSB = frequence_MSB | 0x8000;
		// 使用频率寄存器1输出波形
		AD9833_MULT_Write1(frequence_LSB); // L14，选择频率寄存器1的低14位输入
		AD9833_MULT_Write1(frequence_MSB); // H14 频率寄存器1为
		AD9833_MULT_Write1(Phs_data);	   // 设置相位
										   // AD9833_MULT_Write1(0x2800); 			//设置FSELECT位为0，设置FSELECT位为1，即使用频率寄存器1的值，芯片进入工作状态,频率寄存器1输出波形
	}

	if (WaveMode == TRI_WAVE) // 输出三角波波形
		AD9833_MULT_Write1(0x2002);
	if (WaveMode == SQU_WAVE) // 输出方波波形
		AD9833_MULT_Write1(0x2028);
	if (WaveMode == SIN_WAVE) // 输出正弦波形
		AD9833_MULT_Write1(0x2000);
}
void AD9833_MULT_WaveSeting2(double Freq, unsigned int Freq_SFR, unsigned int WaveMode, unsigned int Phase)
{

	int frequence_LSB, frequence_MSB, Phs_data;
	double frequence_mid, frequence_DATA;
	long int frequence_hex;

	/*********************************计算频率的16进制值***********************************/

	frequence_mid = 268435456 / 25; // 适合25M晶振
									// 如果时钟频率不为25MHZ，修改该处的频率值，单位MHz ，AD9833最大支持25MHz

	frequence_DATA = Freq;
	frequence_DATA = frequence_DATA / 1000000;
	frequence_DATA = frequence_DATA * frequence_mid;
	frequence_hex = frequence_DATA;			// 这个frequence_hex的值是32位的一个很大的数字，需要拆分成两个14位进行处理；
	frequence_LSB = frequence_hex;			// frequence_hex低16位送给frequence_LSB
	frequence_LSB = frequence_LSB & 0x3fff; // 去除最高两位，16位数换去掉高位后变成了14位
	frequence_MSB = frequence_hex >> 14;	// frequence_hex高16位送给frequence_HSB
	frequence_MSB = frequence_MSB & 0x3fff; // 去除最高两位，16位数换去掉高位后变成了14位

	Phs_data = Phase | 0xC000;	// 相位值
	AD9833_MULT_Write2(0x0100); // 复位AD9833,即RESET位为1
	AD9833_MULT_Write2(0x2100); // 选择数据一次写入，B28位和RESET位为1

	if (Freq_SFR == 0) // 把数据设置到设置频率寄存器0
	{
		frequence_LSB = frequence_LSB | 0x4000;
		frequence_MSB = frequence_MSB | 0x4000;
		// 使用频率寄存器0输出波形
		AD9833_MULT_Write2(frequence_LSB); // L14，选择频率寄存器0的低14位数据输入
		AD9833_MULT_Write2(frequence_MSB); // H14 频率寄存器的高14位数据输入
		AD9833_MULT_Write2(Phs_data);	   // 设置相位
										   // AD9833_MULT_Write2(0x2000); 			//设置FSELECT位为0，芯片进入工作状态,频率寄存器0输出波形
	}
	if (Freq_SFR == 1) // 把数据设置到设置频率寄存器1
	{
		frequence_LSB = frequence_LSB | 0x8000;
		frequence_MSB = frequence_MSB | 0x8000;
		// 使用频率寄存器1输出波形
		AD9833_MULT_Write2(frequence_LSB); // L14，选择频率寄存器1的低14位输入
		AD9833_MULT_Write2(frequence_MSB); // H14 频率寄存器1为
		AD9833_MULT_Write2(Phs_data);	   // 设置相位
										   // AD9833_MULT_Write2(0x2800); 			//设置FSELECT位为0，设置FSELECT位为1，即使用频率寄存器1的值，芯片进入工作状态,频率寄存器1输出波形
	}

	if (WaveMode == TRI_WAVE) // 输出三角波波形
		AD9833_MULT_Write2(0x2002);
	if (WaveMode == SQU_WAVE) // 输出方波波形
		AD9833_MULT_Write2(0x2028);
	if (WaveMode == SIN_WAVE) // 输出正弦波形
		AD9833_MULT_Write2(0x2000);
}

#endif
