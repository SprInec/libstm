/**
 ******************************************************************************
 * @file		:bsp_ad7606.c
 * @brief		:AD7606 driver.
 * @version		:0.1.0
 * @author		:凌智电子
 * @date		:2022.07.05
 ******************************************************************************
 * @pinset
 *	
 *	控制线:
 *				rst 	 		 -->				PB3
 *				convstB 	 -->		 		PB4
 *				convstA 	 -->		 		PB5
 *				STby 	     -->		 		PB6
 *				OSI2 	     -->		 		PB7
 *				OSI1 	     -->		 		PB8
 *				OSI0 	     -->		 		PB9
 *
 *				frstdata 	 -->		  	PE2
 *				busy 	  	 -->				PE3
 *				cs 	  		 -->				PE4
 *				rd 	  		 -->		 		PE5
 *
 *	数据线:
 *				DB0 DB1 DB2 DB3 DB4 DB5 DB6 DB7 DB8 DB9 DB10 DB11 DB12 DB13 DB14 DB15 
 *				PF0 PF1 PF2 PF3 PF4 PF5 PF6 PF7 PF8 PF9 PF10 PF11 PF12 PF13 PF14 PF15
 *
 *	外接(并行下选择以下两位):
 *				SER             0V
 *				D15             0V（并行模式下为数据位不用置0）
 *
 * @attention
 *
 *       (1) 并行：SER = 0；D15 = 0；
 *			 (2) 通过J3跳帽选择输入电压的范围。
 *			 (3) J2跳帽这里连接3.3V，具体选择看引脚功能。
 *
 ******************************************************************************
 */

#include "bsp_ad7606.h"
#include "bsp_delay.h"


#if  __AD7606_FSMC_
/*
-----------------------------------------------------------------
 初始化程序区
-----------------------------------------------------------------
-----------------------------------------------------------------
 void GPIO_AD7606_Configuration(void)
-----------------------------------------------------------------

 函数功能: AD7606引脚配置函数
 入口参数: 无
 返回参数: 无
 全局变量: 无
 调用模块: __HAL_RCC_GPIOA_CLK_ENABLE(); HAL_GPIO_DeInit();
 注意事项: 用GPIO驱动方式和FSMC驱动方式下的引脚配置不一样			
-----------------------------------------------------------------
*/
void GPIO_AD7606_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	// 使能IO口时钟
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_5 );//将GPIOx外设寄存器初始化为默认复位值
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
	HAL_GPIO_DeInit(GPIOE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
	HAL_GPIO_DeInit(GPIOF,GPIO_PIN_All);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// AD7606 
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 控制线配置 
	//             CS_N       RD/SCLK      
			GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5;
			GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	//             FRSTDATA     BUSY  
			GPIO_InitStructure.Pin = GPIO_PIN_2 ;
			GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
			GPIO_InitStructure.Mode = GPIO_MODE_EVT_RISING_FALLING;
			GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	//    rst convstB convstA STby OS12 OS11 OS10
      GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | 
			                         GPIO_PIN_5 | GPIO_PIN_6 |
															 GPIO_PIN_7 | GPIO_PIN_8 | 
			                         GPIO_PIN_9 ;
			GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_All, GPIO_PIN_RESET);
    //数据线配置(16)
		//DB0 DB1 DB2 DB3 DB4 DB5 DB6 DB7 DB8 DB9 DB10 DB11 DB12 DB13 DB14 DB15 
		  GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | 
			                         GPIO_PIN_2 | GPIO_PIN_3 |
															 GPIO_PIN_4 | GPIO_PIN_5 | 
			                         GPIO_PIN_6 | GPIO_PIN_7 |
                               GPIO_PIN_8 | GPIO_PIN_9 | 
			                         GPIO_PIN_10 | GPIO_PIN_11 |
															 GPIO_PIN_12 | GPIO_PIN_13 |
			                         GPIO_PIN_14 | GPIO_PIN_15;
			GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
			GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);		
}


/*
-----------------------------------------------------------------
 void AD7606_Init(void)
-----------------------------------------------------------------

 函数功能: AD7606初始化函数
 入口参数: 无
 返回参数: 无
 全局变量: 无
 调用模块:    
 注意事项: 无
-----------------------------------------------------------------
*/
void AD7606_Init(void)
{
	convstA_Set;
	convstB_Set;
	STby_Set;
	rd_Set;
	cs_Set;	
  OS10_Reset;
	OS11_Reset;
	OS12_Reset;
	AD7606_reset();  
	AD7606_startconvst();
	
}

/*   * 名称：AD7606_startconvst()  * 功能：启动转换  */  
void AD7606_startconvst(void)
{  
	convstA_Reset;	
	convstB_Reset;	
	HAL_Delay_us(4);
	convstA_Set;
	convstB_Set;
}
  
/*   * 名称：AD7606_reset()  * 功能：复位模块  */
void AD7606_reset(void) 
{ 
	rst_Reset;
	delay_100ns();
	rst_Set; 
	HAL_Delay_us(1);
	rst_Reset; 
}  

uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((uint16_t)GPIOx->IDR);
}

/* 
* 名称：AD7606_read_data() 
* 功能：读取数据 
* 返回值：返回一个结构体指针，该指针为指向结构体数组的首地址  
*/ 
void AD7606_read_data(int16_t * DB_data) 
{  
	uint8_t i,j; 	
	uint16_t DB_data_H;
	uint16_t DB_data_L;

	HAL_Delay_us(3);
	cs_Reset; 
	HAL_Delay_us (3);
	for(i=0;i<8;i++)  
	{
		rd_Reset;
		HAL_Delay_us (4);		
		rd_Set;
		HAL_Delay_us (4);		
		DB_data[i] = GPIO_ReadInputData(GPIOF);
	}	
	cs_Set;	
} 

#else

AD7606_Class_T AD7606Class_1;

void GPIO_AD7606_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PF11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

static void AD7606_SetOs(uint8_t _ucOS)
{
	AD7606Class_1.ucOS = _ucOS;
	switch(_ucOS)
	{
		case (AD_OS_X2):
											OS2_0();
											OS1_0();
											OS0_1();
											break;
		case (AD_OS_X4):
											OS2_0();
											OS1_1();
											OS0_0();
											break;
		case (AD_OS_X8):
											OS2_0();
											OS1_1();
											OS0_1();
											break;
		case (AD_OS_X16):
											OS2_1();
											OS1_0();
											OS0_0();
											break;
		case (AD_OS_X32):
											OS2_1();
											OS1_0();
											OS0_1();
											break;
		case (AD_OS_X64):
											OS2_1();
											OS1_1();
											OS0_0();
											break;
		case AD_OS_NO:
		default: 
											AD7606Class_1.ucOS = AD_OS_NO;
											OS2_0();
											OS1_0();
											OS0_0();
											break;
	}
}
#if (USE_IO_CONTROL_RANGE)
/*
*********************************************************************************************************
*	@ 函 数 名: AD7606_SetInputRange
*	@ 功能说明: 配置AD7606模拟信号输入量程。
*	@ 形    参: _ucRange : 0 表示正负5V   1表示正负10V
*	@ 返 回 值: 无
*********************************************************************************************************
*/
static void AD7606_SetInputRange(uint8_t _ucRange)
{
	if (_ucRange == AD7606_5V)
	{
		AD7606Class_1.ucRange = AD7606_5V;
		RANGE_0();	/* 设置为正负5V */
	}
	else
	{
		AD7606Class_1.ucRange = AD7606_10V;
		RANGE_1();	/* 设置为正负10V */
	}
}
#endif

/*
*********************************************************************************************************
*	@ 函 数 名: AD7606_Reset
*	@ 功能说明: AD7606硬件复位
*	@ 形    参: NULL
*	@ 返 回 值: 无
*********************************************************************************************************
*/

static void AD7606_Reset(void)
{
	RESET_0();	/* 退出复位状态 */

	RESET_1();	/* 进入复位状态 */
	RESET_1();	/* 仅用于延迟。 RESET复位高电平脉冲宽度最小50ns。 */
	RESET_1();
	RESET_1();
	
	RESET_0();	/* 退出复位状态 */
}
/*
*********************************************************************************************************
*	函 数 名: AD7606_StartConvst
*	功能说明: 启动1次ADC转换
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AD7606_StartConvst(void)
{
	/* page 7：  CONVST 高电平脉冲宽度和低电平脉冲宽度最短 25ns */
	/* CONVST平时为高 */
#if  1
	COA_0_COB_0();
	COA_0_COB_0();
	COA_0_COB_0();

	COA_1_COB_1();			//所用通道
#else
	PAout(5) = 0;
	PAout(6) = 0;
	HAL_Delay_us(11);
	PAout(5) = 1;
	PAout(6) = 1;
#endif
}

static void AD7606_ReadNowAdc(void)
{
	AD7606Class_1.ucADCData[0]	= AD7606_RESULT();	/* 读第1路样本 */
	AD7606Class_1.ucADCData[1]	= AD7606_RESULT();	/* 读第2路样本 */
	AD7606Class_1.ucADCData[2]	= AD7606_RESULT();	/* 读第3路样本 */
	AD7606Class_1.ucADCData[3]	= AD7606_RESULT();	/* 读第4路样本 */
	AD7606Class_1.ucADCData[4]	= AD7606_RESULT();	/* 读第5路样本 */
	AD7606Class_1.ucADCData[5]	= AD7606_RESULT();	/* 读第6路样本 */
	AD7606Class_1.ucADCData[6]	= AD7606_RESULT();	/* 读第7路样本 */
	AD7606Class_1.ucADCData[7]	= AD7606_RESULT();	/* 读第8路样本 */

	//AD7606_SEGGER_RTTOUT();
}
int countsss = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == AD7606_BUSY_Pin)
	{
		
		AD7606_ReadNowAdc();//AD7606转换ISR
#if		0
		PRINTF_DEBUG("adc: %d,%d\n", AD7606Class_1.ucADCData[0]\
															 , AD7606Class_1.ucADCData[1] );
#endif
		countsss++;
		if(countsss > 1000)
			countsss = 0;
	}
	 AD7606_StartConvst();
}
void bsp_InitAD7606(void)
{
	AD7606_SetOs(AD_OS_NO);		/* 无过采样 */
	AD7606_SetInputRange(1);	/* 0表示输入量程为正负5V, 1表示正负10V */
	AD7606_Reset();						/* 复位 */
	COA_1_COB_0();						/* 启动转换的GPIO，平时设置为高 */
}
void BSP_AD7606_Init(void)
{
	bsp_InitAD7606();
	AD7606_SetOs(AD_OS_NO);
	AD7606_SetInputRange(1);	/* 0表示输入量程为正负5V, 1表示正负10V */
	AD7606_StartConvst();			/* 启动1次转换 */
}




#endif


