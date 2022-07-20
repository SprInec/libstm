/**
 ******************************************************************************
 * @file		:bsp_config.h
 * @brief		:global control file for the BSP library.
 * @version		:0.1.3
 * @author		:July
 * @date		:2022.06.25
 ******************************************************************************
 *@attention
 *
 *This file is the global control file for the BSP library.
 *The user could select the relevant files and functions that enable the BSP
 *library by setting the macro definition in this file.
 *
 ******************************************************************************
 */

#ifndef __BSP_CONFIG__
#define __BSP_CONFIG__

/* !< CHIP TYPE */
#if			1
	#define __BSP_STM32F1
#else
	#define __BSP_STM32F4
#endif

/* SYS */
#if			0
	#define __BSP_Sys
#endif

/* DELAY */
#if			0
	#define __BSP_Delay
#endif

/* DWT */
#if			0
	#define __BSP_Dwt
#endif

#if			0
	#define __BSP_SysTick
#endif

/* TIM */
#if			1
	#define __BSP_Tim
#endif

/* !< BSP USART */
#if			1
	#define __BSP_Usart
#endif 

/* !< BSP LED */
#if			1
	#define __BSP_Led
#endif

/* !< BSP BUZZER */
#if			0
	#define __BSP_Buzzer
#endif

/* !< BSP COMMON */
#if			1
	#define __BSP_Common
#endif

/* !< PERIPHERALS SELECTION */

/* DDS */
/* AD9833 */
#if			1
#define __BSP_AD9833
#endif

/* AD9959 */
#if			1
	#define __BSP_AD9959
#endif

/* AD7606 */
#if			0
	#define __BSP_AD7606
#endif

/* DAC8563 */
#if			0
	#define __BSP_DAC8563
#endif

/* PLL */
#if			0
	#define __BSP_ADF4351
#endif

/* THERMOMETRY */
#if			0
	#define __BSP_DS18B20
#endif

/* !< DISPLAY EQUIPMENT */
#if			0
	#define __BSP_Lcd1602
#endif

#if			1
	#define __BSP_HMI
#endif

#if			0
	#define __BSP_OLED
#endif

/* !< ANALOG */
/* ADC */
#if			0
	#define __BSP_ADC
#endif

/* DAC */
#if			1
	#define __BSP_DAC
#endif
  
/* !< DIGITAL CODING */
/* CODING */
#if			0
	#define __BSP_Coding
#endif

/* DECODE */
#if			0
	#define __BSP_Decode
#endif

/* FILTER */
#if			0
	#define __BSP_Filter
#endif

/* FFT */
#if			0
	#define __BSP_FFT
#endif

/* MATH */
#if			0
	#define __BSP_Math
#endif



#if			1
	#include "bsp_user.h"
#endif	/* User Code */

#if			1
	#include "stdint.h"
#endif /*  All rights reserved */

#ifdef __BSP_Sys
	#include "bsp_sys.h"
#endif /* __BSP_Sys */

#ifdef __BSP_Delay
	#include "bsp_delay.h"
#endif /* __BSP_Delay */

#ifdef __BSP_Dwt
	#include "bsp_dwt.h"
#endif /* __BSP_Dwt */

#ifdef __BSP_SysTick
	#include "bsp_SysTick.h"
#endif /* __BSP_SysTick */

#ifdef __BSP_Tim
	#include "bsp_tim.h"
#endif /* __BSP_Tim */

#ifdef __BSP_Usart
	#include "bsp_usart.h"
#endif /* __BSP_Usart */

#ifdef __BSP_Buzzer
	#include "bsp_buzzer.h"
#endif /* __BSP_Buzzer */

#ifdef __BSP_Led
	#include "bsp_led.h"
#endif /* __BSP_Led */

#ifdef __BSP_Common
	#include "bsp_common.h"
#endif /* __BSP_Commeon */

#ifdef __BSP_HMI
	#include "bsp_hmi.h"
#endif /* __BSP_USART_HMI */

#ifdef __BSP_OLED
	#include "bsp_oled.h"
#endif /* __BSP_OLED */

#ifdef __BSP_AD9833
	#include "bsp_ad9833.h"
#endif /* __BSP_AD9833 */

#ifdef __BSP_AD9959
	#include "bsp_ad9959.h"
#endif /* __BSP_AD9959 */

#ifdef __BSP_AD7606
	#include "bsp_ad7606.h"
#endif	/* __BSP_AD7606 */

#ifdef __BSP_DAC8563
	#include "bsp_dac8563.h"
#endif /* __BSP_DAC8563 */

#ifdef __BSP_DS18B20
	#include "bsp_ds18b20.h"
#endif /* __BSP_DS18B20 */

#ifdef __BSP_Lcd1602
	#include "bsp_lcd1602.h"
#endif /* __BSP_Lcd1602 */

#ifdef __BSP_ADC
	#include "bsp_adc.h"
#endif /* __BSP_ADC */

#ifdef __BSP_DAC
	#include "bsp_dac.h"
#endif /* __BSP_DAC */

#ifdef __BSP_Coding
	#include "bsp_coding.h"
#endif /* __BSP_Coding */

#ifdef __BSP_Decode
	#include "bsp_decode.h"
#endif /* __BSP_Decode */

#ifdef __BSP_Filter
	#include "bsp_filter.h"
#endif	/* __BSP_Filter */

#ifdef __BSP_FFT
	#include "bsp_fft.h"
#endif	/* __BSP_FFT */

#ifdef __BSP_Math
	#include "bsp_math.h"
#endif	/* __BSP_Math */
	


#endif
