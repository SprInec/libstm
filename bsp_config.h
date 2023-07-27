/**
 * @file bsp_config.h
 * @author July (Email: JulyCub@163.com)
 * @brief Global control file for the BSP library.
 * @version 2.1
 * @date 2022.05.25
 * @update 2023.05.24 - Add some drivers
 * @update 2023.07.03 - Update and optimize some code
 * @update 2023.07.15 - Add CMSIS Configuration Wizard support
 * @attention 
 *  -->
 *  This file is the global control file for the BSP library.
 *  The user could select the relevant files and functions that enable the BSP
 *  library by setting the macro definition in this file.
 * 
 * @copyright Copyright (c) 2023 July
 *
 */

#ifndef __BSP_CONFIG__
#define __BSP_CONFIG__

// <<< Use Configuration Wizard in Context Menu >>>
//
// <h> System configuration
// ========================================
//  <o> Development board and chip selection
//      <1=> DevEBox-STM32F103C6T6
//      <2=> DevEBox-STM32F407VET6
//      <3=> DevEBox-STM32H743VIT6
//      <4=> NUCLEO-H7A3ZI-Q
//  <i> Select the development board and chip type
#ifndef MCU_SELECTION
#define MCU_SELECTION 2
#endif

// MCU AUTO-DEFINE
#if MCU_SELECTION == 1
#define __BSP_MCU_DEVEBOX_STM32F103C6T6
#elif MCU_SELECTION == 2
#define __BSP_MCU_DEVEBOX_STM32F407VET6
#elif MCU_SELECTION == 3
#define __BSP_MCU_DEVEBOX_STM32H743VIT6
#elif MCU_SELECTION == 4
#define __BSP_MCU_NUCLEO_H7A3ZIQ
#endif 

// <e> RTOS
// ========
//  <e> RT-Thread
//  <i> Enable RT-Thread OS
//  <i> Default: 0
#ifndef __RTOS_RTTHREAD_ENABLED
#define __RTOS_RTTHREAD_ENABLED 0
#endif 
// </e> !RT-Thread
// </e> !RTOS

// <e> CMSIS DSP
#ifndef __CMSIS_DSP
#define __CMSIS_DSP 0
#endif
// </e> !main.h

// <e> main.h
#ifndef __BSP_CITE_MAIN
#define __BSP_CITE_MAIN 1
#endif
// </e> !main.h

// <e> bsp_user.h
#ifndef __BSP_CITE_USER
#define __BSP_CITE_USER 1
#endif
// </e> !bsp_user.h
// </h> !System configuration

// <h> C Standard library files
// ============================
//  <i> Select the c standard library file you want to use
//  <i> Default: NOUSE

// <e> stdio.h
#ifndef __BSP_C_STD_STDIO
#define __BSP_C_STD_STDIO 1
#endif
// </e> !stdint.h

//  <e> stdint.h
#ifndef __BSP_C_STD_STDINT
#define __BSP_C_STD_STDINT 0
#endif
// </e> !stdio.h

// <e> math.h
#ifndef __BSP_C_STD_MATH
#define __BSP_C_STD_MATH 0
#endif
// </e> !math.h
// </h> !C Standard library files

// <h> std support files
// =====================
// <e> bsp_sys.h
//  <i> Default: USING
#ifndef __BSP_SYS_ENABLED
#define __BSP_SYS_ENABLED 1
#endif
// </e>

// <e> bsp_common.h
//  <i> Default: USING
#ifndef __BSP_COMMON_ENABLED
#define __BSP_COMMON_ENABLED 1
#endif
// </e>

// <e> bsp_delay.h
//  <i> Default: USING
#ifndef __BSP_DELAY_ENABLED
#define __BSP_DELAY_ENABLED 1
#endif
// </e>
// </h> !std support files

// <h> BSP on-chip Peripheral
// ==========================
// <e> BSP ADC
#ifndef __BSP_ADC_ENABLED
#define __BSP_ADC_ENABLED 0
#endif
// </e>

// <e> BSP DAC
#ifndef __BSP_DAC_ENABLED
#define __BSP_DAC_ENABLED 0
#endif
// </e>

// <e> BSP TIM
#ifndef __BSP_TIM_ENABLED
#define __BSP_TIM_ENABLED 0
#endif
// </e>

// <e> BSP DWT
#ifndef __BSP_DWT_ENABLED
#define __BSP_DWT_ENABLED 0
#endif
// </e>
// </h> !BSP on-chip Peripheral

// <h> Peripheral drive
// ====================
//  <i> Select the peripheral module driver you want to use
//  <i> Default: NOUSE

// <e> LED
//  <i> Default: USING
#ifndef __BSP_LED_ENABLED
#define __BSP_LED_ENABLED 1
#endif
// </e>

// <e> Buzzer
#ifndef __BSP_BUZZER_ENABLED
#define __BSP_BUZZER_ENABLED 0
#endif
// </e>

// <e> DS18B20
#ifndef __BSP_DS18B20_ENABLED
#define __BSP_DS18B20_ENABLED 0
#endif
// </e>

// <h> Programmed DDS
// ==================
//  <i> Select the programmed DDS driver you want to use
//  <i> Default: NOUSE

// <e> AD9833
#ifndef __BSP_AD9833_ENABLED
#define __BSP_AD9833_ENABLED 0
#endif
// </e>

// <e> AD9851
#ifndef __BSP_AD9851_ENABLED
#define __BSP_AD9851_ENABLED 0
#endif
// </e>

// <e> AD9854
#ifndef __BSP_AD9854_ENABLED
#define __BSP_AD9854_ENABLED 1
#endif
// </e>

// <e> AD9959
#ifndef __BSP_AD9959_ENABLED
#define __BSP_AD9959_ENABLED 1
#endif
// </e>
// </h> !Programmed DDS

// <h> Programmed ADC
// ==================
// <e> AD7606
#ifndef __BSP_AD7606_ENABLED
#define __BSP_AD7606_ENABLED 0
#endif
// </e>
// </h> !Programmed ADC

// <h> Programmed DAC
// ==================
// <e> DAC8563
#ifndef __BSP_DAC8563_ENABLED
#define __BSP_DAC8563_ENABLED 0
#endif
// </e>
// </h> !Programmed DAC

// <h> Programmed PLL
// ==================
// <e> ADF4351
#ifndef __BSP_ADF4351_ENABLED
#define __BSP_ADF4351_ENABLED 0
#endif
// </e>

// <e> ADF4002
#ifndef __BSP_ADF4002_ENABLED
#define __BSP_ADF4002_ENABLED 0
#endif
// </e>
// </h> !Programmed PLL

// <e> LMK61E07
//  <i> One dcxo module
#ifndef __BSP_LMK61E07_ENABLED
#define __BSP_LMK61E07_ENABLED 0
#endif
// </e>

// <e> PE4302
#ifndef __BSP_PE4302_ENABLED
#define __BSP_PE4302_ENABLED 0
#endif
// </e>

// <h> Display screen selection
// ============================
// <e> OLED I2C
#ifndef __BSP_OLEDI2C_ENABLED
#define __BSP_OLEDI2C_ENABLED 0
#endif
// </e>

// <e> OLED SPI
#ifndef __BSP_OLEDSPI_ENABLED
#define __BSP_OLEDSPI_ENABLED 0
#endif
// </e>

// <e> LCD1602
#ifndef __BSP_LCD1602_ENABLED
#define __BSP_LCD1602_ENABLED 0
#endif
// </e>

// <e> LCD TFT 
#ifndef __BSP_LCDTFT_ENABLED
#define __BSP_LCDTFT_ENABLED 0
#endif
// </e>

// <e> UART HMI
#ifndef __BSP_UARTHMI_ENABLED
#define __BSP_UARTHMI_ENABLED 0
#endif
// </e>
// </h> !Display screen selection

// <h> Camera
// ==========
// <e> OV2640
#ifndef __BSP_OV2640_ENABLED
#define __BSP_OV2640_ENABLED 0
#endif
// </e>
// </h> !Camera
// </h> !Peripheral drive

// <h> external storage
// ====================
// <i> Select the external storage media you use

// <e> EEPROM
#ifndef __BSP_EEPROM_ENABLED
#define __BSP_EEPROM_ENABLED 0
#endif
// </e>

// <e> W25QXX
#ifndef __BSP_W25QXX_ENABLED
#define __BSP_W25QXX_ENABLED 0
#endif
// </e>

// </h> !external storage

// <h> Communication Protocol
// ==========================
// <e> BSP I2C
#ifndef __BSP_I2C_ENABLED
#define __BSP_I2C_ENABLED 0
#endif
// </e>

// <e> BSP SPI
#ifndef __BSP_SPI_ENABLED
#define __BSP_SPI_ENABLED 0
#endif
// </e>

// <e> BSP SCCB
#ifndef __BSP_SCCB_ENABLED
#define __BSP_SCCB_ENABLED 0
#endif
// </e>

// <e> BSP USART
#ifndef __BSP_USART_ENABLED
#define __BSP_USART_ENABLED 1
#endif
// </e>

// <e> BSP QUADSPI
#ifndef __BSP_QUADSPI_ENABLED
#define __BSP_QUADSPI_ENABLED 0
#endif
// </e>
// </h> !Communication Protocol

// <h> Custom algorithm
// ====================
// <e> CODING
#ifndef __BSP_CODING_ENABLED
#define __BSP_CODING_ENABLED 0
#endif
// </e>

// <e> DEDING
#ifndef __BSP_DEDING_ENABLED
#define __BSP_DEDING_ENABLED 0
#endif
// </e>

// <e> FILTER
#ifndef __BSP_FILTER_ENABLED
#define __BSP_FILTER_ENABLED 0
#endif
// </e>

// <e> BSP MATH
#ifndef __BSP_MATH_ENABLED
#define __BSP_MATH_ENABLED 0
#endif
// </e>

// <e> FFT
#ifndef __BSP_FFT_ENABLED
#define __BSP_FFT_ENABLED 0
#endif
// </e>

// <e> SPWM
#ifndef __BSP_SPWM_ENABLED
#define __BSP_SPWM_ENABLED 0
#endif
// </e>
// </h> !Custom algorithm
// <<< end of configuration section >>>

/*!======= BSP Files Include =======!*/

#ifdef __BSP_MCU_DEVEBOX_STM32F103C6T6
#include "stm32f1xx_hal.h"
#endif /* !__BSP_MCU_DEVEBOX_STM32F103C6T6 */

#ifdef __BSP_MCU_DEVEBOX_STM32F407VET6
#include "stm32f4xx_hal.h"
#endif /* !__BSP_MCU_DEVEBOX_STM32F407VET6 */

#ifdef __BSP_MCU_DEVEBOX_STM32H743VIT6
#include "stm32h7xx_hal.h"
#endif /* !__BSP_MCU_DEVEBOX_STM32H743VIT6 */

#ifdef __BSP_MCU_NUCLEO_H7A3ZIQ
#include "stm32h7xx_hal.h"
#endif /* !__BSP_MCU_NUCLEO_H7A3ZIQ */

#if __RTOS_RTTHREAD_ENABLED
#include "rtthread.h"
#endif /* !RTOS_RTTHREAD_ENABLED */

#if __BSP_CITE_MAIN
#include "main.h"
#endif /* !MAIN */

#if __BSP_CITE_USER
#include "bsp_user.h"
#endif /* !USER CODE */

#if __BSP_C_STD_STDIO
#include "stdio.h"
#endif /* !STDIO */

#if __BSP_C_STD_STDINT
#include "stdint.h"
#endif /* !STDINT */

#if __BSP_C_STD_MATH
#include "math.h"
#endif /* !MATH */

#if __BSP_SYS_ENABLED
#include "bsp_sys.h"
#endif /* !__BSP_SYS_ENABLED */

#if __BSP_COMMON_ENABLED
#include "bsp_common.h"
#endif /* !__BSP_COMMON_ENABLED */

#if __BSP_DELAY_ENABLED
#include "bsp_delay.h"
#endif /* !__BSP_DELAY_ENABLED */

#if __BSP_ADC_ENABLED
#include "bsp_adc.h"
#endif /* !__BSP_ADC_ENABLED */

#if __BSP_DAC_ENABLED
#include "bsp_dac.h"
#endif /* !__BSP_DAC_ENABLED */

#if __BSP_TIM_ENABLED
#include "bsp_tim.h"
#endif /* !__BSP_TIM_ENABLED */

#if __BSP_DWT_ENABLED
#include "bsp_dwt.h"
#endif /* !__BSP_DWT_ENABLED */

#if __BSP_LED_ENABLED
#include "bsp_led.h"
#endif /* !__BSP_LED_ENABLED */

#if __BSP_BUZZER_ENABLED
#include "bsp_buzzer.h"
#endif /* !__BSP_BUZZER_ENABLED */

#if __BSP_DS18B20_ENABLED
#include "bsp_ds18b20.h"
#endif /* !__BSP_DS18B20_ENABLED */

#if __BSP_AD9833_ENABLED
#include "bsp_ad9833.h"
#endif /*! __BSP_AD9833_ENABLED */

#if __BSP_AD9851_ENABLED
#include "bsp_ad9851.h"
#endif /* !__BSP_AD9851_ENABLED */

#if __BSP_AD9854_ENABLED
#include "bsp_ad9854.h"
#endif /* !__BSP_AD9854_ENABLED */

#if __BSP_AD9959_ENABLED
#include "bsp_ad9959.h"
#endif /* !__BSP_AD9959_ENABLED */

#if __BSP_AD7606_ENABLED
#include "bsp_ad7606.h"
#endif /* !__BSP_AD7606_ENABLED */

#if __BSP_DAC8563_ENABLED
#include "bsp_dac8563.h"
#endif /* !__BSP_DAC8563_ENABLED */

#if __BSP_ADF4351_ENABLED
#include "bsp_adf4351.h"
#endif /* !__BSP_ADF4351_ENABLED */

#if __BSP_ADF4002_ENABLED
#include "bsp_adf4002.h"
#endif /* !__BSP_ADF4002_ENABLED */

#if __BSP_LMK61E07_ENABLED
#include "bsp_lmk61e07.h"
#endif /* !__BSP_LMK61E07_ENABLED */

#if __BSP_PE4302_ENABLED
#include "bsp_pe4302.h"
#endif /* !__BSP_PE4302_ENABLED */

#if __BSP_OLEDI2C_ENABLED
#include "bsp_oled_i2c.h"
#endif /* !__BSP_OLEDI2C_ENABLED */

#if __BSP_OLEDSPI_ENABLED
#include "bsp_oled_spi.h"
#endif /* !__BSP_OLEDI2C_ENABLED */

#if __BSP_LCD1602_ENABLED
#include "bsp_lcd1602.h"
#endif /* !__BSP_LCD1602_ENABLED */

#if __BSP_LCDTFT_ENABLED
#include "bsp_lcd_tft.h"
#endif /* !__BSP_LCD_ENABLED */

#if __BSP_UARTHMI_ENABLED
#include "bsp_usart_hmi.h"
#endif /* !__BSP_HMI_ENABLED */

#if __BSP_OV2640_ENABLED
#include "bsp_ov2640.h"
#endif /* !__BSP_OV2640_ENABLED */

#if __BSP_EEPROM_ENABLED
#include "bsp_eeprom.h"
#endif /* !__BSP_EEPROM_ENABLED */

#if __BSP_W25QXX_ENABLED
#include "bsp_w25qxx.h"
#endif /* !__BSP_W25QXX_ENABLED */

#if __BSP_I2C_ENABLED
#include "bsp_i2c.h"
#endif /* !__BSP_I2C_ENABLED */

#if __BSP_SPI_ENABLED
#include "bsp_spi.h"
#endif /* !__BSP_I2C_ENABLED */

#if __BSP_SCCB_ENABLED
#include "bsp_sccb.h"
#endif /* !__BSP_SCCB_ENABLED */

#if __BSP_USART_ENABLED
#include "bsp_usart.h"
#endif /* !__BSP_USART_ENABLED */

#if __BSP_QUADSPI_ENABLED
#include "bsp_quadspi.h"
#endif /* !__BSP_QUADSPI_ENABLED */

#if __BSP_CODING_ENABLED
#include "bsp_encode.h"
#endif /*! __BSP_CODING_ENABLED */

#if __BSP_DECODE_ENABLED
#include "bsp_decode.h"
#endif /* !__BSP_DECODE_ENABLED */

#if __BSP_FILTER_ENABLED
#include "bsp_filter.h"
#endif /* !__BSP_FILTER_ENABLED */

#if __BSP_MATH_ENABLED
#include "bsp_math.h"
#endif /* !__BSP_MATH_ENABLED */

#if __BSP_FFT_ENABLED
#include "bsp_fft.h"
#endif /* !__BSP_FFT_ENABLED */

#if __BSP_SPWM_ENABLED
#include "bsp_spwm.h"
#endif /* !__BSP_SPWM_ENABLED */

#endif /* !__BSP_CONFIG__ */
