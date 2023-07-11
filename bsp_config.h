/**
 ******************************************************************************
 * @file			: bsp_config.h
 * @brief			: global control file for the BSP library.
 * @version		    : 2.0
 * @author		    : July
 * @date			: v0.1 -> 2022.06.25
 * @update          : v1.0 -> 2023.05.24
 * @update          : v2.0 -> 2023.07.03
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

/*!======= BSP Files Enable =======!*/

/* !< CHIP TYPE >! */
/* -> F103 */
#if 0
#define __BSP_STM32F1_ENABLED
#endif

/* -> F407 */
#if 1
#define __BSP_STM32F4_ENABLED
#endif

#if 0
#define __BSP_STM32G4_ENABLED
#endif

/* -> H7A3 */
#if 0
#define __BSP_STM32H7_ENABLED
#endif

/*!< RTOS >!*/
#if 0
#define __RTOS_RTTHREAD_ENABLED
#endif

/* !< MAIN >! */
#if 1
#define __MAIN_ENABLED
#endif

/* !< USER CODE >! */
#if 0
#define __BSP_USER_ENABLED
#endif

/* !< STDINT >! */
#if 1
#define __STDINT_ENABLED
#endif

/* !< SYS >! */
#if 1
#define __BSP_SYS_ENABLED
#endif

/* !< COMMON >! */
#if 1
#define __BSP_COMMON_ENABLED
#endif

/* !< BSP DELAY >! */
#if 1
#define __BSP_DELAY_ENABLED
#endif

/* !< DWT >! */
#if 0
#define __BSP_DWT_ENABLED
#endif

/* !< BSP TIM >! */
#if 0
#define __BSP_TIM_ENABLED
#endif

/* !< BSP USART >! */
#if 1
#define __BSP_USART_ENABLED
#endif

/* !< LED >! */
#if 1
#define __BSP_LED_ENABLED
#endif

/* !< BUZZER >! */
#if 0
#define __BSP_BUZZER_ENABLED
#endif

/* !< EEPORM >! */
#if 0
#define __BSP_EEPORM_ENABLED
#endif

/* !< PERIPHERALS SELECTION */
/* !< DDS >! */
/* AD9833 */
#if 0
#define __BSP_AD9833_ENABLED
#endif

/* AD9851 */
#if 0
#define __BSP_AD9851_ENABLED
#endif

/* AD9959 */
#if 0
#define __BSP_AD9959_ENABLED
#endif

/* AD7606 */
#if 0
#define __BSP_AD7606_ENABLED
#endif

/* DAC8563 */
#if 0
#define __BSP_DAC8563_ENABLED
#endif

/* !< PLL >! */
/* ADF4351 */
#if 0
#define __BSP_ADF4351_ENABLED
#endif

/* ADF4002 */
#if 0
#define __BSP_ADF4002_ENABLED
#endif

/* !< DCXO >! */
/* LMK61E07 */
#if 1
#define __BSP_LMK61E07_ENABLED
#endif

/* PE4302 */
#if 1
#define __BSP_PE4302_ENABLED
#endif 

/* !< THERMOMETRY >! */
/* DS18B20 */
#if 0
#define __BSP_DS18B20_ENABLED
#endif

/* !< DISPLAY EQUIPMENT */
/* LCD */
#if 0
#define __BSP_LCD_ENABLED
#endif

/* BSP DCMI */
#if 0
#define __BSP_DCMI_ENABLED
#endif

/* LCD1602 */
#if 0
#define __BSP_LCD1602_ENABLED
#endif

/* HMI */
#if 0
#define __BSP_HMI_ENABLED
#endif

/* OLED */
#if 0
#define __BSP_OLED_ENABLED
#endif

/* !< CAMERA */
/* OV2640 */
#if 0
#define __BSP_OV2640_ENABLED
#endif

/* !< ANALOG */
/* ADC */
#if 0
#define __BSP_ADC_ENABLED
#endif

/* DAC */
#if 0
#define __BSP_DAC_ENABLED
#endif

/* SPWM */
#if 0
#define __BSP_SPWM_ENABLED
#endif

/* !< DIGITAL CODING */
/* CODING */
#if 0
#define __BSP_CODING_ENABLED
#endif

/* DECODE */
#if 0
#define __BSP_DECODE_ENABLED
#endif

/* FILTER */
#if 0
#define __BSP_FILTER_ENABLED
#endif

/* FFT */
#if 0
#define __BSP_FFT_ENABLED
#endif

/* MATH */
#if 0
#define __BSP_MATH_ENABLED
#endif

/* !< COMMUNICATION >! */
/* SCCB */
#if 0
#define __BSP_SCCB_ENABLED
#endif

/* IIC */
#if 1
#define __BSP_I2C_ENABLED
#endif

/* SPI */
#if 0
#define __BSP_SPI_ENABLED
#endif

/*!======= BSP Files Include =======!*/

#ifdef __BSP_STM32F1_ENABLED
#include "stm32f1xx_hal.h"
#endif

#ifdef __BSP_STM32F4_ENABLED
#include "stm32f4xx_hal.h"
#endif

#ifdef __BSP_STM32G4_ENABLED
#include "stm32g4xx_hal.h"
#endif

#ifdef __BSP_STM32H7_ENABLED
#include "stm32h7xx_hal.h"
#endif

#ifdef __MAIN_ENABLED
#include "main.h"
#endif /* !MAIN */

#ifdef __BSP_USER_ENABLED
#include "bsp_user.h"
#endif /* !USER CODE */

#ifdef __STDINT_ENABLED
#include "stdint.h"
#endif /*  !ALL RIGHTS RESERVED */

#ifdef __RTOS_RTTHREAD_ENABLED
#include "rtthread.h"
#endif /* !RTOS_RTTHREAD_ENABLED */

#ifdef __BSP_SYS_ENABLED
#include "bsp_sys.h"
#endif /* !__BSP_SYS_ENABLED */

#ifdef __BSP_DELAY_ENABLED
#include "bsp_delay.h"
#endif /* !__BSP_DELAY_ENABLED */

#ifdef __BSP_DWT_ENABLED
#include "bsp_dwt.h"
#endif /* !__BSP_DWT_ENABLED */

#ifdef __BSP_TIM_ENABLED
#include "bsp_tim.h"
#endif /* !__BSP_TIM_ENABLED */

#ifdef __BSP_USART_ENABLED
#include "bsp_usart.h"
#endif /* !__BSP_USART_ENABLED */

#ifdef __BSP_BUZZER_ENABLED
#include "bsp_buzzer.h"
#endif /* !__BSP_BUZZER_ENABLED */

#ifdef __BSP_EEPROM_ENABLED
#include "bsp_eeprom.h"
#endif /* !__BSP_EEPROM_ENABLED */

#ifdef __BSP_LED_ENABLED
#include "bsp_led.h"
#endif /* !__BSP_LED_ENABLED */

#ifdef __BSP_COMMON_ENABLED
#include "bsp_common.h"
#endif /* !__BSP_COMMON_ENABLE */

#ifdef __BSP_HMI_ENABLED
#include "bsp_hmi.h"
#endif /* !__BSP_HMI_ENABLED */

#ifdef __BSP_OLED_ENABLED
#include "bsp_oled.h"
#endif /* !__BSP_OLED_ENABLED */

#ifdef __BSP_AD9833_ENABLED
#include "bsp_ad9833.h"
#endif /*! __BSP_AD9833_ENABLED */

#ifdef __BSP_AD9851_ENABLED
#include "bsp_ad9851.h"
#endif /* !__BSP_AD9851_ENABLED */

#ifdef __BSP_AD9959_ENABLED
#include "bsp_ad9959.h"
#endif /* !__BSP_AD9959_ENABLED */

#ifdef __BSP_AD7606_ENABLED
#include "bsp_ad7606.h"
#endif /* !__BSP_AD7606_ENABLED */

#ifdef __BSP_ADF4351_ENABLED
#include "bsp_adf4351.h"
#endif /* !__BSP_ADF4351_ENABLED */

#ifdef __BSP_ADF4002_ENABLED
#include "bsp_adf4002.h"
#endif /* !__BSP_ADF4002_ENABLED */

#ifdef __BSP_LMK61E07_ENABLED
#include "bsp_lmk61e07.h"
#endif /* !__BSP_LMK61E07_ENABLED */

#ifdef __BSP_PE4302_ENABLED
#include "bsp_pe4302.h"
#endif /* !__BSP_PE4302_ENABLED */

#ifdef __BSP_DAC8563_ENABLED
#include "bsp_dac8563.h"
#endif /* !__BSP_DAC8563_ENABLED */

#ifdef __BSP_DS18B20_ENABLED
#include "bsp_ds18b20.h"
#endif /* !__BSP_DS18B20_ENABLED */

#ifdef __BSP_LCD_ENABLED
#include "bsp_lcd.h"
#endif /* !__BSP_LCD_ENABLED */

#ifdef __BSP_DCMI_ENABLED
#include "bsp_dcmi.h"
#endif /* !__BSP_DCMI_ENABLED */

#ifdef __BSP_LCD1602_ENABLED
#include "bsp_lcd1602.h"
#endif /* !__BSP_LCD1602_ENABLED */

#ifdef __BSP_OV2640_ENABLED
#include "bsp_ov2640.h"
#endif /* !__BSP_OV2640_ENABLED */

#ifdef __BSP_ADC_ENABLED
#include "bsp_adc.h"
#endif /* !__BSP_ADC_ENABLED */

#ifdef __BSP_DAC_ENABLED
#include "bsp_dac.h"
#endif /* !__BSP_DAC_ENABLED */

#ifdef __BSP_SPWM_ENABLED
#include "bsp_spwm.h"
#endif /* !__BSP_SPWM_ENABLED */

#ifdef __BSP_CODING_ENABLED
#include "bsp_coding.h"
#endif /*! __BSP_CODING_ENABLED */

#ifdef __BSP_DECODE_ENABLED
#include "bsp_decode.h"
#endif /* !__BSP_DECODE_ENABLED */

#ifdef __BSP_FILTER_ENABLED
#include "bsp_filter.h"
#endif /* !__BSP_FILTER_ENABLED */

#ifdef __BSP_FFT_ENABLED
#include "bsp_fft.h"
#endif /* !__BSP_FFT_ENABLED */

#ifdef __BSP_MATH_ENABLED
#include "bsp_math.h"
#endif /* !__BSP_MATH_ENABLED */

#ifdef __BSP_SCCB_ENABLED
#include "bsp_sccb.h"
#endif /* !__BSP_SCCB_ENABLED */

#ifdef __BSP_I2C_ENABLED
#include "bsp_i2c.h"
#endif /* !__BSP_I2C_ENABLED */

#endif
