/**
 ******************************************************************************
 * @file			: bsp_config.h
 * @brief			: global control file for the BSP library.
 * @version		    : 0.1.3
 * @author		    : July
 * @date			: 2022.06.25
 ******************************************************************************
 * @attention
 *
 *	This file is the global control file for the BSP library.
 *	The user could select the relevant files and functions that enable the BSP
 *	library by setting the macro definition in this file.
 *
 ******************************************************************************
 */

#ifndef __BSP_CONFIG__
#define __BSP_CONFIG__

/* !< CHIP TYPE */
#if			0
#define __BSP_STM32F1_ENABLED
#else
#define __BSP_STM32F4_ENABLED
#endif

/* SYS */
#if			0
#define __BSP_SYS_ENABLED
#endif

/* DELAY */
#if			1
#define __BSP_DELAY_ENABLED
#endif

/* DWT */
#if			0
#define __BSP_DWT_ENABLED
#endif

#if			0
#define __BSP_SYSTICK_ENABLED
#endif

/* TIM */
#if			0
#define __BSP_TIM_ENABLED
#endif

/* !< BSP USART */
#if			1
#define __BSP_USART_ENABLED
#endif 

/* !< BSP LED */
#if			1
#define __BSP_LED_ENABLED
#endif

/* !< BSP BUZZER */
#if			0
#define __BSP_BUZZER_ENABLED
#endif

/* !< BSP COMMON */
#if			1
#define __BSP_COMMON_ENABLE
#endif

/* !< PERIPHERALS SELECTION */

/* DDS */
/* AD9833 */
#if			0
#define __BSP_AD9833_ENABLED
#endif

/* AD9851 */
#if         0
#define __BSP_AD9851_ENABLED
#endif

/* AD9959 */
#if			0
#define __BSP_AD9959_ENABLED
#endif

/* AD7606 */
#if			0
#define __BSP_AD7606_ENABLED
#endif

/* DAC8563 */
#if			0
#define __BSP_DAC8563_ENABLED
#endif

/* PLL */
#if			0
#define __BSP_ADF4351_ENABLED
#endif

#if			0
#define __BSP_ADF4002_ENABLED
#endif

/* THERMOMETRY */
#if			0
#define __BSP_DS18B20_ENABLED
#endif

/* !< DISPLAY EQUIPMENT */
#if			0
#define __BSP_LCD1602_ENABLED
#endif

#if			0
#define __BSP_HMI_ENABLED
#endif

#if			0
#define __BSP_OLED_ENABLED
#endif

/* !< ANALOG */
/* ADC */
#if			0
#define __BSP_ADC_ENABLED
#endif

/* DAC */
#if			0
#define __BSP_DAC_ENABLED
#endif
  
/* !< DIGITAL CODING */
/* CODING */
#if			0
#define __BSP_CODING_ENABLED
#endif

/* DECODE */
#if			0
#define __BSP_DECODE_ENABLED
#endif

/* FILTER */
#if			0
#define __BSP_FILTER_ENABLED
#endif

/* FFT */
#if			0
#define __BSP_FFT_ENABLED
#endif

/* MATH */
#if			0
#define __BSP_MATH_ENABLED
#endif

#if         0
#include "main.h"
#endif /* MAIN */

#if			0
#include "bsp_user.h"
#endif	/* USER CODE */

#if			1
#include "stdint.h"
#endif /*  ALL RIGHTS RESERVED */

#ifdef __BSP_SYS_ENABLED
#include "bsp_sys.h"
#endif /* __BSP_SYS_ENABLED */

#ifdef __BSP_DELAY_ENABLED
#include "bsp_delay.h"
#endif /* __BSP_DELAY_ENABLED */

#ifdef __BSP_DWT_ENABLED
#include "bsp_dwt.h"
#endif /* __BSP_DWT_ENABLED */

#ifdef __BSP_SYSTICK_ENABLED
#include "bsp_SysTick.h"
#endif /* __BSP_SYSTICK_ENABLED */

#ifdef __BSP_TIM_ENABLED
#include "bsp_tim.h"
#endif /* __BSP_TIM_ENABLED */

#ifdef __BSP_USART_ENABLED
#include "bsp_usart.h"
#endif /* __BSP_USART_ENABLED */

#ifdef __BSP_BUZZER_ENABLED
#include "bsp_buzzer.h"
#endif /* __BSP_BUZZER_ENABLED */

#ifdef __BSP_LED_ENABLED
#include "bsp_led.h"
#endif /* __BSP_LED_ENABLED */

#ifdef __BSP_COMMON_ENABLE
#include "bsp_common.h"
#endif /* __BSP_COMMON_ENABLE */

#ifdef __BSP_HMI_ENABLED
#include "bsp_hmi.h"
#endif /* __BSP_HMI_ENABLED */

#ifdef __BSP_OLED_ENABLED
#include "bsp_oled.h"
#endif /* __BSP_OLED_ENABLED */

#ifdef __BSP_AD9833_ENABLED
#include "bsp_ad9833.h"
#endif /* __BSP_AD9833_ENABLED */

#ifdef __BSP_AD9851_ENABLED
#include "bsp_ad9851.h"
#endif /* __BSP_AD9851_ENABLED */

#ifdef __BSP_AD9959_ENABLED
#include "bsp_ad9959.h"
#endif /* __BSP_AD9959_ENABLED */

#ifdef __BSP_AD7606_ENABLED
#include "bsp_ad7606.h"
#endif	/* __BSP_AD7606_ENABLED */

#ifdef __BSP_ADF4351_ENABLED
#include "bsp_adf4351.h"
#endif /* __BSP_ADF4351_ENABLED */

#ifdef __BSP_ADF4002_ENABLED
#include "bsp_adf4002.h"
#endif /* __BSP_ADF4002_ENABLED */

#ifdef __BSP_DAC8563_ENABLED
#include "bsp_dac8563.h"
#endif /* __BSP_DAC8563_ENABLED */

#ifdef __BSP_DS18B20_ENABLED
#include "bsp_ds18b20.h"
#endif /* __BSP_DS18B20_ENABLED */

#ifdef __BSP_LCD1602_ENABLED
#include "bsp_lcd1602.h"
#endif /* __BSP_LCD1602_ENABLED */

#ifdef __BSP_ADC_ENABLED
#include "bsp_adc.h"
#endif /* __BSP_ADC_ENABLED */

#ifdef __BSP_DAC_ENABLED
#include "bsp_dac.h"
#endif /* __BSP_DAC_ENABLED */

#ifdef __BSP_CODING_ENABLED
#include "bsp_coding.h"
#endif /* __BSP_CODING_ENABLED */

#ifdef __BSP_DECODE_ENABLED
#include "bsp_decode.h"
#endif /* __BSP_DECODE_ENABLED */

#ifdef __BSP_FILTER_ENABLED
#include "bsp_filter.h"
#endif	/* __BSP_FILTER_ENABLED */

#ifdef __BSP_FFT_ENABLED
#include "bsp_fft.h"
#endif	/* __BSP_FFT_ENABLED */

#ifdef __BSP_MATH_ENABLED
#include "bsp_math.h"
#endif	/* __BSP_MATH_ENABLED */
	


#endif
