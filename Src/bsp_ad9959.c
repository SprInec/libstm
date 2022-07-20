/**
 ******************************************************************************
 * @file			:bsp_ad9959.c
 * @brief			:The board support package for AD9959.
 * @version		:0.1.1
 * @author		:July
 * @date			:2022.07.01
 * @updata		:2022.07.14
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
 *							GPIOC_PIN_7 	---> P0
 *							GPIOD_PIN_7 	---> P1
 *							GPIOC_PIN_10	---> P2
 *							GPIOD_PIN_2		---> P3
 *							GPIOA_PIN_8		---> SD0
 *							GPIOC_PIN_9		---> SD1
 *							GPIOG_PIN_7		---> SD2
 *							GPIOG_PIN_6		---> SD3
 *							GPIOD_PIN_6		---> PDC/PWR
 *							GPIOC_PIN_11	---> UPDATA
 *							GPIOG_PIN_15	---> RST
 *							GPIOC_PIN_12	---> CS
 *							GPIOC_PIN_8		---> SCK
 *
 *******************************************************************************
 */
 
#include "bsp_ad9959.h"
#include "gpio.h"

#if 	AD9959_USART_DRIVE
uint8_t		ad9959_mode[][10] = {"POINT", "SWEEP", "FSK2", "FSK4", "AM"};
uint32_t	ad9959_param = 0;
#else

uint32_t 	FRE_Send = 0;

uint8_t 	AD9959_Mode = 0;							//扫频或者点频
uint8_t 	AD9959_Wave_Show_Mode = 1;		//跟随扫频--1  不跟随扫频--0


/********************************扫频/扫相*********************************/
/*
  * 通用
 * */
uint8_t  	AD9959_SweepWaveFlag = 0;    	//是否显示波形
uint32_t 	AD9959_SweepCount = 0;    		//extern到定时器中--ms
uint32_t 	AD9959_SweepCountTimes = 0;   //extern到定时器中
uint32_t 	AD9959_SweepTime = 2;    			//扫频间隔时间--ms


/*
  * 扫频
 * */
uint32_t 	AD9959_SweepMaxFre  = 1000000; 	//最大扫频频率--Hz
uint32_t	AD9959_SweepMinFre  =    1000;  //最小扫频频率--Hz
uint32_t 	AD9959_SweepStepFre =    1000;  //扫频步进频率--Hz


/*
  * 扫相  0~16383(对应角度：0°~360°)
 * */
uint32_t 	AD9959_SweepMaxPha = 16383; 	//最大扫描相位
uint32_t	AD9959_SweepMinPha =  0;  		//最小扫描相位
uint32_t 	AD9959_SweepStepPha =  450;   //扫相步进相位


/********************************点频*********************************/
uint32_t 	AD9959_FixedMaxFre = 300000; 	//最大固定输出频率--Hz
uint32_t	AD9959_FixedMinFre = 10000;  	//最小固定输出频率--Hz
uint32_t 	AD9959_FixedStepFre = 10000; 	//步进固定输出频率--Hz
uint32_t 	AD9959_FixedAmpli = 1023; 	  //最大输出频率幅度--Hz
uint32_t 	AD9959_FixedNowFre = 15000; 	//此时输出频率--Hz


uint32_t 	AD9959_NowSinFre[5] = {0};
uint32_t 	AD9959_NowSinAmp[5] = {0};
uint32_t 	AD9959_NowSinPhr[5] = {0};


int AD9959_FixedPhase_Differ = 0; 	    //相位
int AD9959_FixedPhase_Differ_Last = 0; 	//相位
uint8_t AD9959_FixedAmpli_Differ = 0; 	//幅度
int AD9959_FixedAmpli_Eror = -40;				//相位计算校准




uint8_t CSR_DATA0[1] = {0x10};      // 开 CH0
uint8_t CSR_DATA1[1] = {0x20};      // 开 CH1
uint8_t CSR_DATA2[1] = {0x40};      // 开 CH2
uint8_t CSR_DATA3[1] = {0x80};      // 开 CH3


uint8_t FR2_DATA[2] = {0x00,0x00};			//default Value = 0x0000
uint8_t CFR_DATA[3] = {0x00,0x03,0x02};	//default Value = 0x000302

uint8_t CPOW0_DATA[2] = {0x00,0x00};		//default Value = 0x0000   @ = POW/2^14*360



uint8_t LSRR_DATA[2] = {0x00,0x00};					//default Value = 0x----

uint8_t RDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------

uint8_t FDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------


uint32_t SinFre[5] = {40000, 40000, 40000, 40000};
uint32_t SinAmp[5] = {1023, 1023, 1023, 1023};
uint32_t SinPhr[5] = {0, 455, 4096*2-1, 4096*3-1};

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
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
	                          |GPIO_PIN_7, GPIO_PIN_RESET);

	  /*Configure GPIOC pin Output Level */
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|
													GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

	  GPIO_InitStruct.Pin = GPIO_PIN_15;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|
													GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
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
	  __HAL_RCC_GPIOF_CLK_ENABLE();
	  __HAL_RCC_GPIOH_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();


	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOG, AD9959_SDIO3_Pin|AD9959_SDIO2_Pin|AD9959_Reset_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOC, AD9959_PS0_Pin|AD9959_SCLK_Pin|AD9959_SDIO1_Pin|AD9959_PS2_Pin
	                          |AD9959_UPDATE_Pin|AD9959_CS_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(AD9959_SDIO0_GPIO_Port, AD9959_SDIO0_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOD, AD9959_PS3_Pin|AD9959_PWR_Pin|AD9959_PS1_Pin, GPIO_PIN_RESET);


	  /*Configure GPIO pins : PGPin PGPin PGPin */
	  GPIO_InitStruct.Pin = AD9959_SDIO3_Pin|AD9959_SDIO2_Pin|AD9959_Reset_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
	                           PCPin PCPin */
	  GPIO_InitStruct.Pin = AD9959_PS0_Pin|AD9959_SCLK_Pin|AD9959_SDIO1_Pin|AD9959_PS2_Pin
	                          |AD9959_UPDATE_Pin|AD9959_CS_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  /*Configure GPIO pin : PtPin */
	  GPIO_InitStruct.Pin = AD9959_SDIO0_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(AD9959_SDIO0_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pins : PDPin PDPin PDPin */
	  GPIO_InitStruct.Pin = AD9959_PS3_Pin|AD9959_PWR_Pin|AD9959_PS1_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
#endif

//延时
void delay1 (uint32_t length)
{
	length = length*12;
   while(length--);
}
//IO口初始化
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
//AD9959复位
void IntReset(void)
{
	AD9959_Reset_RESET;
	delay1(1);//1
	AD9959_Reset_SET;
	delay1(30);//30
	AD9959_Reset_RESET;
}
 //AD9959更新数据
void IO_Update(void)
{
	AD9959_UPDATE_RESET;
	delay1(2); //2
	AD9959_UPDATE_SET;
	delay1(4); //4
	AD9959_UPDATE_RESET;
}
/*--------------------------------------------
函数功能：控制器通过SPI向AD9959写数据
RegisterAddress: 寄存器地址
NumberofRegisters: 所含字节数
*RegisterData: 数据起始地址
temp: 是否更新IO寄存器
----------------------------------------------*/
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData,uint8_t temp)
{
	uint8_t	ControlValue = 0;
	uint8_t	ValueToWrite = 0;
	uint8_t	RegisterIndex = 0;
	uint8_t	i = 0;

	ControlValue = RegisterAddress;
	//写入地址
	AD9959_SCLK_RESET;
	AD9959_CS_RESET;
	for(i=0; i<8; i++)
	{
		AD9959_SCLK_RESET;
		if(0x80 == (ControlValue & 0x80))
			AD9959_SDIO0_SET;
		else
			AD9959_SDIO0_RESET;
		AD9959_SCLK_SET;
		ControlValue <<= 1;
	}
	AD9959_SCLK_RESET;
	//写入数据
	for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i=0; i<8; i++)
		{
			AD9959_SCLK_RESET;
			if(0x80 == (ValueToWrite & 0x80))
				AD9959_SDIO0_SET;
			else
				AD9959_SDIO0_RESET;
			AD9959_SCLK_SET;
			ValueToWrite <<= 1;
		}
		AD9959_SCLK_RESET;
	}
	if(temp != 0)
		IO_Update();
	AD9959_CS_SET;
}
/*---------------------------------------
函数功能：设置通道输出频率
Channel:  输出通道
Freq:     输出频率
---------------------------------------*/
void Write_frequence(uint8_t Channel,uint32_t Freq)
{
		uint8_t CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
	  uint32_t Temp;
	  Temp=(uint32_t)Freq*8.589934592;	   //将输入频率因子分为四个字节  8.589934592=(2^32)/500000000 其中500M=25M*20(倍频数可编程)
	  CFTW0_DATA[3]=(uint8_t)Temp;
	  CFTW0_DATA[2]=(uint8_t)(Temp>>8);
	  CFTW0_DATA[1]=(uint8_t)(Temp>>16);
	  CFTW0_DATA[0]=(uint8_t)(Temp>>24);
	  if(Channel==0)
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);//控制寄存器写入CH0通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH0设定频率
	  }
	  else if(Channel==1)
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);//控制寄存器写入CH1通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH1设定频率
	  }
	  else if(Channel==2)
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);//控制寄存器写入CH2通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH2设定频率
	  }
	  else if(Channel==3)
	  {
			WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);//控制寄存器写入CH3通道
			WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);//CTW0 address 0x04.输出CH3设定频率
	  }

}
/*---------------------------------------
函数功能：设置通道输出幅度
Channel:  输出通道
Ampli:    输出幅度
---------------------------------------*/
void Write_Amplitude(uint8_t Channel, uint16_t Ampli)
{
	uint16_t A_temp;//=0x23ff;
	uint8_t ACR_DATA[3] = {0x00,0x00,0x00};//default Value = 0x--0000 Rest = 18.91/Iout
	A_temp=Ampli|0x1000;
	ACR_DATA[2] = (uint8_t)A_temp;  //低位数据
	ACR_DATA[1] = (uint8_t)(A_temp>>8); //高位数据

	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}

	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}

	else if(Channel==2)
	{
	  WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);
	  WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}

	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);
		WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
	}
}
/*---------------------------------------
函数功能：设置通道输出相位
Channel:  输出通道
Phase:    输出相位,范围：0~16383(对应角度：0°~360°)
---------------------------------------*/
void Write_Phase(uint8_t Channel,uint16_t Phase)
{
	uint16_t P_temp=0;
	P_temp=(uint16_t)Phase;
	CPOW0_DATA[1]=(uint8_t)P_temp;
	CPOW0_DATA[0]=(uint8_t)(P_temp>>8);
	if(Channel==0)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,1);
	}

	else if(Channel==1)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,1);
	}

	else if(Channel==2)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,1);
	}

	else if(Channel==3)
	{
		WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);
		WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,1);
	}
}


//AD9959初始化
void AD9959_Init(void)
{
	uint8_t FR1_DATA[3] = {0xD0,0x00,0x00};//20倍频 Charge pump control = 75uA FR1<23> -- VCO gain control =0时 system clock below 160 MHz;

	AD9959_Drv_Init();

	Intserve();  //IO口初始化
	IntReset();  //AD9959复位

  WriteData_AD9959(FR1_ADD,3,FR1_DATA,1);//写功能寄存器1
  WriteData_AD9959(FR2_ADD,2,FR2_DATA,1);

#if 0
  WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
  WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
  WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
  WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);
  WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);
#endif

  //写入初始频率
	Write_frequence(3,SinFre[3]);
	Write_frequence(0,SinFre[0]);
	Write_frequence(1,SinFre[1]);
	Write_frequence(2,SinFre[2]);

	Write_Phase(3, SinPhr[3]);
	Write_Phase(0, SinPhr[0]);
	Write_Phase(1, SinPhr[1]);
	Write_Phase(2, SinPhr[2]);

	Write_Amplitude(3, SinAmp[3]);
	Write_Amplitude(0, SinAmp[0]);
	Write_Amplitude(1, SinAmp[1]);
	Write_Amplitude(2, SinAmp[2]);
}
#endif


