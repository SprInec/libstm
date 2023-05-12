/*
 ******************************************************************************
 * @file		    :bsp_adf4002.h
 * @brief		    :ADF4002 Driver.
 * @version		    :0.1.0
 * @author		    :July
 * @date		    :2022.07.27
 ******************************************************************************
 * @pinset
 *						PC6		--> SCK
 *						PC7		--> SDA/SDI
 *						PC8		--> LE/SEN
 *
 ******************************************************************************
 */
#ifndef __F_ADF4002_H
#define __F_ADF4002_H

#include "bsp_config.h"

/* ========================*/
#define USE_ADF4002_VER_1
/* ========================*/

#ifdef USE_ADF4002_VER_1

#define R_Address 0X000000
#define N_Address 0X000001
#define F_Address 0X000002
#define I_Address 0X000003
#define Pre_R 0X000000          //X000,0000,DAT(14),00
#define Pre_N 0X000000          //XX0,DAT(13),XXXXXX,01

//管脚声明
//GND
#define PLL_SCK PCout(6)
#define PLL_SDI PCout(7)
#define PLL_SEN PCout(8)

#define PLL_SCK_0 PLL_SCK = 0
#define PLL_SCK_1 PLL_SCK = 1

#define PLL_SDI_0 PLL_SDI = 0
#define PLL_SDI_1 PLL_SDI = 1

#define PLL_SEN_0 PLL_SEN = 0
#define PLL_SEN_1 PLL_SEN = 1


void ADF4002_InitPll(void);
void testADF4002(void);
void SendDataPll(unsigned long int Data);
void SetFrequency(uint32_t fvco);
void RDivideTest(uint16_t RData);

#endif

#ifdef USE_ADF4002_VER_2
/*-----------------------接口定义----------------------*/

#define ADF4002_SCK_GPIO_Port			GPIOC
#define ADF4002_SCK_Pin						GPIO_PIN_6

#define ADF4002_SDA_GPIO_Port			GPIOC
#define ADF4002_SDA_Pin						GPIO_PIN_7

#define ADF4002_LE_GPIO_Port			GPIOC
#define ADF4002_LE_Pin						GPIO_PIN_8

#define ADF4002_SCK_1  ADF4002_SCK_GPIO_Port->ODR|=ADF4002_SCK_Pin
#define ADF4002_SCK_0  ADF4002_SCK_GPIO_Port->ODR&=~ADF4002_SCK_Pin		

#define ADF4002_SDA_1  ADF4002_SDA_GPIO_Port->ODR|=ADF4002_SDA_Pin
#define ADF4002_SDA_0  ADF4002_SDA_GPIO_Port->ODR&=~ADF4002_SDA_Pin	

#define ADF4002_LE_1   ADF4002_LE_GPIO_Port->ODR|=ADF4002_LE_Pin
#define ADF4002_LE_0   ADF4002_LE_GPIO_Port->ODR&=~ADF4002_LE_Pin		

/*-----------------------寄存器定义----------------------*/
//========== Control Bits ================
#define CTRL_R_COUNTER       0x000000
#define CTRL_N_COUNTER       0x000001
#define CTRL_FUNC_LATCH      0x000002
#define CTRL_INIT_LATCH      0x000003

//========== R Counter Register ==========
//LOCK DETECT PRECISION
#define RCOUNTER_LDP 	     0x100000

//Antibacklash pause width
#define RCOUNTER_ABP_2_9_ns  0x000000
#define RCOUNTER_ABP_DISABLE 0x010000
#define RCOUNTER_ABP_6_ns    0x020000

//========== N Counter Register ==========
//CP Gain ,需要结合“F4”寄存器位来使用
#define NCOUNTER_G1 		 0x200000

//======== Initialization latch ==========
// Power Down 设定 下面是在CE引脚为1的情况下
// CE为0 时为异步掉电模式 
#define INIT_PD_DISABLE   	 0x000000  //正常操作
#define INIT_PD_SYNC   	 	 0x000008  //同步掉电
#define INIT_PD_ASYNC   	 0x200008  //异步掉电

// Current setting
#define INIT_CPI2(x)		 (x<<18)
#define INIT_CPI1(x)		 (x<<15)

// Timer counter control
#define INIT_TC(x) 			 (x<<11)

// Fastlock mode
#define INIT_FASTLOCK_DISABLE 0x000000
#define INIT_FASTLOCK_MODE_1  0x000200
#define INIT_FASTLOCK_MODE_2  0x000600

// Charge pump output
#define INIT_F3 			  0x000100	 //1:three-state,0:mormal

// Phase detector polarity
#define INIT_F2 			  0x000080	 //1:positive	,0:negative

// Muxout control
#define INIT_MUX_THREE_STATE  0x000000
#define INIT_MUX_LOCK_DETECT  0x000010
#define INIT_MUX_N_DIVIDER    0x000020
#define INIT_MUX_DVDD  	      0x000030
#define INIT_MUX_R_DIVIDER    0x000040
#define INIT_MUX_OPENDRAIN    0x000050
#define INIT_MUX_SDA_OUT      0x000060
#define INIT_MUX_DGND         0x000070

// Counter operation
#define INIT_F1			      0x000004	 //1: R&N counter reset,0:normal

/*-----------------------函数定义----------------------*/
void ADF4002_Init ();
void ADF4002_SetRCounter (uint16_t _num);
void ADF4002_SetNCounter (uint16_t _num);
void ADF4002_WriteLatch (uint32_t _register);
void ADF4002_SetFrequency (uint64_t _freq);
#endif

#endif
