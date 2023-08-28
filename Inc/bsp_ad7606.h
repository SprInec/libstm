/**
 ******************************************************************************
 * @file		:bsp_ad7606.h
 * @brief		:AD7606 driver.
 * @version	:0.1.0
 * @author	:凌智电子
 * @date		:2022.07.05
 ******************************************************************************
 * @pinset
 *	
 *	控制线 
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
 *	数据线
 *				DB0 DB1 DB2 DB3 DB4 DB5 DB6 DB7 DB8 DB9 DB10 DB11 DB12 DB13 DB14 DB15 
 *				PF0 PF1 PF2 PF3 PF4 PF5 PF6 PF7 PF8 PF9 PF10 PF11 PF12 PF13 PF14 PF15
 *
 *	外接（并行下选择以下两位）
 *				SER             0V
 *				D15             0V（并行模式下为数据位不用置0）
 *
 *@attention
 *
 *       (1)并行：SER = 0；D15 = 0；
 *			 (2) 通过J3跳帽选择输入电压的范围，修改程序第115行中sprintf里面的
 *					 (float)(DB_data[i]*5000.0/32768)的5000.0，在5000.0和10000.0根据跳帽改变。
 *			 (3) J2跳帽这里连接3.3V，具体选择看引脚功能。
 *
 ******************************************************************************
 */

#ifndef __BSP_AD7606_H__
#define __BSP_AD7606_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

/*=================================================*/
	#define __AD7606_FSMC_					1
/*=================================================*/


#if  __AD7606_FSMC_

#define rst_Set 	 		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define rst_Reset  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)
#define convstB_Set 	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
#define convstB_Reset   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define convstA_Set 	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define convstA_Reset   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define STby_Set 	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
#define STby_Reset 	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
#define OS12_Set 	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
#define OS12_Reset      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)
#define OS11_Set 	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)
#define OS11_Reset      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)
#define OS10_Set 	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)
#define OS10_Reset      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)


#define frstdata_Set 	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET)
#define frstdata_Reset  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET)
#define busy_Set 	  		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
#define busy_Reset   		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define cs_Set 	  			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET)
#define cs_Reset   			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET)
#define rd_Set 	  		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET)
#define rd_Reset   		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET)

extern void GPIO_AD7606_Configuration(void);
extern void AD7606_Init(void);
extern void AD7606_startconvst(void);
extern void AD7606_reset(void) ;
extern void AD7606_read_data(int16_t * DB_data);

#else

#define   USE_IO_CONTROL_RANGE     1
#define   USE_AD7606_CHANGLE       8

/* 控制引脚定义 */
#define AD7606_COA_Pin 						GPIO_PIN_5
#define AD7606_COA_GPIO_Port 			GPIOA
#define AD7606_COB_Pin 						GPIO_PIN_6
#define AD7606_COB_GPIO_Port 			GPIOA
#define AD7606_BUSY_Pin 					GPIO_PIN_4
#define AD7606_BUSY_GPIO_Port 		GPIOC
#define AD7606_BUSY_EXTI_IRQn 		EXTI4_IRQn
#define AD7606_OS2_Pin 						GPIO_PIN_5
#define AD7606_OS2_GPIO_Port 			GPIOC
#define AD7606_OS0_Pin 						GPIO_PIN_0
#define AD7606_OS0_GPIO_Port 			GPIOB
#define AD7606_OS1_Pin 						GPIO_PIN_1
#define AD7606_OS1_GPIO_Port 			GPIOB
#define AD7606_RANGE_Pin 					GPIO_PIN_2
#define AD7606_RANGE_GPIO_Port 		GPIOB
#define AD7606_RST_Pin 						GPIO_PIN_11
#define AD7606_RST_GPIO_Port 			GPIOF


/*os控制引脚定义*/
#define   PINOUTLOW(_PORT_, _PIN_)   do{P##_PORT_##out(_PIN_) = 0;}while(0)
#define   PINOUTHIGH(_PORT_, _PIN_)  do{P##_PORT_##out(_PIN_) = 1;}while(0)
#define 	OS0_0()      PINOUTLOW(B, 0)
#define 	OS1_0()      PINOUTLOW(B, 1)
#define 	OS2_0()      PINOUTLOW(C, 5)
#define 	OS0_1()      PINOUTHIGH(B, 0)
#define 	OS1_1()      PINOUTHIGH(B, 1)
#define 	OS2_1()      PINOUTHIGH(C, 5)
/* AD7606 FSMC总线地址，只能读，无需写 */
#define AD7606_RESULT()	*(__IO uint16_t *)0x6C000000

typedef enum
{
	AD_OS_NO 			= 0,
	AD_OS_X2 			= 1,
	AD_OS_X4 			= 2,
	AD_OS_X8 			= 3,
	AD_OS_X16 		= 4,
	AD_OS_X32 		= 5,
	AD_OS_X64 		= 6
}AD7606_OS_E;
/*采样量程设置*/
#if (USE_IO_CONTROL_RANGE)
#define  AD7606_5V        0
#define  AD7606_10V       1
#define  RANGE_0()    		PINOUTLOW(B, 2)
#define  RANGE_1()    		PINOUTHIGH(B, 2)
#endif
/*复位定义*/
#define	 RESET_0()				PINOUTLOW(F, 11)
#define	 RESET_1()				PINOUTHIGH(F, 11)
/*通道选择控制*/
#define	 COA_0()					PINOUTLOW(A, 5)
#define	 COA_1()					PINOUTHIGH(A, 5)
#define	 COB_0()					PINOUTLOW(A, 6)
#define	 COB_1()					PINOUTHIGH(A, 6)
#define	 COA_0_COB_0()		AD7606_COA_GPIO_Port->BSRR &= !(AD7606_COA_Pin | AD7606_COB_Pin)
#define	 COA_1_COB_1()		AD7606_COA_GPIO_Port->BSRR |= (AD7606_COA_Pin  | AD7606_COB_Pin)
#define	 COA_0_COB_1()		AD7606_COA_GPIO_Port->BSRR |= (AD7606_COA_Pin  | !AD7606_COB_Pin)
#define	 COA_1_COB_0()		AD7606_COA_GPIO_Port->BSRR |= (!AD7606_COA_Pin | AD7606_COB_Pin)



typedef struct{
		uint8_t ucOS;               /*过采样倍率 ， 0 - 6， 0 - 无过采样*/
		uint8_t ucRange;						/*ad7606 输入量程*/
		int16_t ucADCData[USE_AD7606_CHANGLE];       /*ad7606 adc值*/
}AD7606_Class_T;

extern AD7606_Class_T AD7606Class_1;

void GPIO_AD7606_Config(void);
void BSP_AD7606_Init(void);
void bsp_InitAD7606(void);

#endif

#ifdef __cplusplus
}
#endif
#endif
