 /**
 * @file bsp_config.h
 * @author SprInec (julycubspring@gmail.com)
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
 * @copyright Copyright (c) 2023 SprInec. All rights reserved.
 *
 */

#ifndef __BSP_CONFIG__H__
#define __BSP_CONFIG__H__
#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>
//
// <h> System configuration
// ========================================
//  <o> Development board and chip selection
//      <1=> DevEBox-STM32F103C6T6
//      <2=> DevEBox-STM32F407VET6
//      <3=> DevEBox-STM32H743VIT6
//      <4=> NUCLEO-H7A3ZI-Q
//      <5=> LANQIAO-GXCT-STM32G431
//      <6=> USER-DEFINED
//  <i> Select the development board and chip type
#ifndef MCU_SELECTION
#define MCU_SELECTION 6
#endif

// DEV-BOARD AUTO-DEFINE
#if MCU_SELECTION == 1
#define BSP_MCU_DEVEBOX_STM32F103C6T6
#elif MCU_SELECTION == 2
#define BSP_MCU_DEVEBOX_STM32F407VET6
#elif MCU_SELECTION == 3
#define BSP_MCU_DEVEBOX_STM32H743VIT6
#elif MCU_SELECTION == 4
#define BSP_MCU_NUCLEO_H7A3ZIQ
#elif MCU_SELECTION == 5
#define BSP_MCU_LANQIAO_GXCT_STM32G431
#else
#define BSP_MCU_USER_DEFINED
#endif

// <o> MCU ID
//     <0=> STM32F1
//     <1=> STM32F4
//     <2=> STM32H7
//     <3=> STM32G4
// <i> Select the MCU ID
#define MCU_ID 1

#if MCU_ID == 0
#define MCUID_STM32F1
#elif MCU_ID == 1
#define MCUID_STM32F4
#elif MCU_ID == 2
#define MCUID_STM32H7
#elif MCU_ID == 3
#define MCUID_STM32G4
#endif

// <h> RTOS
// =======================
//  <o> Select the RTOS
//      <0=> NOUSE
//      <1=> RT-Thread
//      <2=> FreeRTOS
#define BSP_ENABLE_RTOS 2
// </h> !RTOS

#if BSP_ENABLE_RTOS == 1
#ifndef RTOS_RTTHREAD_ENABLED
#define RTOS_RTTHREAD_ENABLED 1
#endif 
#elif BSP_ENABLE_RTOS == 2
#ifndef RTOS_FREERTOS_ENABLED
#define RTOS_FREERTOS_ENABLED 1
#endif 
#endif

// <e> CMSIS DSP
#ifndef USING_CMSIS_DSP
#define USING_CMSIS_DSP 0
#endif
// </e> !main.h

// <e> main.h
#ifndef BSP_CITE_MAIN
#define BSP_CITE_MAIN 1
#endif
// </e> !main.h

// </h> !System configuration

// <h> C Standard library files
// ============================
//  <i> Select the c standard library file you want to use
//  <i> Default: NOUSE

// <e> stdio.h
#ifndef BSP_C_STD_STDIO
#define BSP_C_STD_STDIO 1
#endif
// </e> !stdint.h

// <e> stdlib.h
#ifndef BSP_C_STD_STDLIB
#define BSP_C_STD_STDLIB 0
#endif
// </e> !stdlib.h

// <e> string.h
#ifndef BSP_C_STD_STRING
#define BSP_C_STD_STRING 1
#endif
// </e> !string.h

//  <e> stdint.h
#ifndef BSP_C_STD_STDINT
#define BSP_C_STD_STDINT 0
#endif
// </e> !stdio.h

// <e> math.h
#ifndef BSP_C_STD_MATH
#define BSP_C_STD_MATH 0
#endif
// </e> !math.h

// <e> limits.h
#ifndef BSP_C_STD_LIMITS
#define BSP_C_STD_LIMITS 1
#endif
// </e> !limits.h
// </h> !C Standard library files

// <h> std support files
// =====================
// <e> bsp_sys.h
//  <i> Default: USING
#ifndef BSP_SYS_ENABLED
#define BSP_SYS_ENABLED 0
#endif
// </e>

// <e> bsp_common.h
//  <i> Default: USING
#ifndef BSP_COMMON_ENABLED
#define BSP_COMMON_ENABLED 1
#endif
// </e>

// <e> bsp_delay.h
//  <i> Default: USING
#ifndef BSP_DELAY_ENABLED
#define BSP_DELAY_ENABLED 1
#endif
// </e>
// </h> !std support files

// <h> BSP on-chip Peripheral
// ==========================
// <e> BSP ADC
#ifndef BSP_ADC_ENABLED
#define BSP_ADC_ENABLED 0
#endif
// </e>

// <e> BSP DAC
#ifndef BSP_DAC_ENABLED
#define BSP_DAC_ENABLED 0
#endif
// </e>

// <e> BSP TIM
#ifndef BSP_TIM_ENABLED
#define BSP_TIM_ENABLED 0
#endif
// </e>

// <e> BSP DWT
#ifndef BSP_DWT_ENABLED
#define BSP_DWT_ENABLED 0
#endif
// </e>
// </h> !BSP on-chip Peripheral

// <h> Peripheral drive
// ====================
//  <i> Select the peripheral module driver you want to use
//  <i> Default: NOUSE

// <e> LED
//  <i> Default: USING
#ifndef BSP_LED_ENABLED
#define BSP_LED_ENABLED 1
#endif
// </e>

// <e> Buzzer
#ifndef BSP_BUZZER_ENABLED
#define BSP_BUZZER_ENABLED 0
#endif
// </e>

// <e> KEY
#ifndef BSP_KEY_ENABLED
#define BSP_KEY_ENABLED 0
#endif
// <e> STATEMAC KEY
#ifndef STATEMAC_KEY
#define STATEMAC_KEY 0
#endif 
// </e> !statemac key
// <e> NORMAL KEY
#ifndef NORMAL_KEY
#define NORMAL_KEY 1
#endif 
// </e> !NORMAL KEY
// </e> !KEY

// <h> Programmed DDS
// ==================
//  <i> Select the programmed DDS driver you want to use
//  <i> Default: NOUSE

// <e> AD9833
#ifndef BSP_AD9833_ENABLED
#define BSP_AD9833_ENABLED 0
#endif
// </e>

// <e> AD9851
#ifndef BSP_AD9851_ENABLED
#define BSP_AD9851_ENABLED 0  
#endif
// </e>

// <e> AD9854
#ifndef BSP_AD9854_ENABLED
#define BSP_AD9854_ENABLED 0
#endif
// </e>

// <e> AD9959
#ifndef BSP_AD9959_ENABLED
#define BSP_AD9959_ENABLED 0
#endif
// </e>
// </h> !Programmed DDS

// <h> Programmed ADC
// ==================
// <e> AD7606
#ifndef BSP_AD7606_ENABLED
#define BSP_AD7606_ENABLED 0
#endif
// </e>
// </h> !Programmed ADC

// <h> Programmed DAC
// ==================
// <e> DAC8563
#ifndef BSP_DAC8563_ENABLED
#define BSP_DAC8563_ENABLED 0
#endif
// </e>
// </h> !Programmed DAC

// <h> Programmed PLL
// ==================
// <e> ADF4351
#ifndef BSP_ADF4351_ENABLED
#define BSP_ADF4351_ENABLED 0
#endif
// </e>

// <e> ADF4002
#ifndef BSP_ADF4002_ENABLED
#define BSP_ADF4002_ENABLED 0
#endif
// </e>
// </h> !Programmed PLL

// <e> LMK61E07
//  <i> One dcxo module
#ifndef BSP_LMK61E07_ENABLED
#define BSP_LMK61E07_ENABLED 0
#endif
// </e>

// <e> PE4302
#ifndef BSP_PE4302_ENABLED
#define BSP_PE4302_ENABLED 0
#endif
// </e>

// <h> Display screen selection
// ============================
// <e> OLED I2C
#ifndef BSP_OLEDI2C_ENABLED
#define BSP_OLEDI2C_ENABLED 0
#endif
// </e>

// <e> OLED SPI
#ifndef BSP_OLEDSPI_ENABLED
#define BSP_OLEDSPI_ENABLED 0
#endif
// </e>

// <e> LCD1602
#ifndef BSP_LCD1602_ENABLED
#define BSP_LCD1602_ENABLED 0
#endif
// </e>

// <e> LCD TFT
#ifndef BSP_LCDTFT_ENABLED
#define BSP_LCDTFT_ENABLED 0
#endif
// </e>

// <e> UART HMI
#ifndef BSP_UARTHMI_ENABLED
#define BSP_UARTHMI_ENABLED 0
#endif
// </e>
// </h> !Display screen selection

// <h> Sensor
// ==========
// <e> DS18B20
#ifndef BSP_DS18B20_ENABLED
#define BSP_DS18B20_ENABLED 0
#endif
// </e>

// <e> DHT11
#ifndef BSP_DHT11_ENABLED
#define BSP_DHT11_ENABLED 0
#endif 
// </e> !DHT11

// <e> DHT20
#ifndef BSP_DHT20_ENABLED
#define BSP_DHT20_ENABLED 0
#endif
// </e> !DHT20

// <e> BMP280
#ifndef BSP_BMP280_ENABLED
#define BSP_BMP280_ENABLED 0
#endif
// </e> !BMP280

// <e> S12SD
#ifndef BSP_S12SD_ENABLED
#define BSP_S12SD_ENABLED 0
#endif 
// </e> !S12SD

// <e> GP12
#ifndef BSP_GP2Y_ENABLED
#define BSP_GP2Y_ENABLED 0
#endif
// </e> GP12

// <e> WTU404F4-B004
//  <i> 高精度红外模块
#ifndef BSP_WTU404F4_ENABLED
#define BSP_WTU404F4_ENABLED 1
#endif
// </e> !WTU404F4

// <e> GPS
#ifndef BSP_GPS_ENABLED
#define BSP_GPS_ENABLED 0
#endif
// </e> !GPS

// <e> VMS WIND SPEED SENSOR
#ifndef BSP_VMSWIND_ENABLED
#define BSP_VMSWIND_ENABLED 0
#endif 
// </e> !VMS WIND SPEED SENSOR

// </h> !Sensor

// <h> Power Monitoring
// <e> INA226
#ifndef BSP_INA226_ENABLED
#define BSP_INA226_ENABLED 1
#endif
// </e> !INA226
// </h> !Power Monitoring

// <h> Motor&Servo
// =============
// <e> TMI8260
#ifndef BSP_TMI8260_ENABLED
#define BSP_TMI8260_ENABLED 1
#endif
// </e> !TMI8260
// </h> !Motor&Servo

// <h> Camera
// ==========
// <e> OV2640
#ifndef BSP_OV2640_ENABLED
#define BSP_OV2640_ENABLED 0
#endif
// </e>
// </h> !Camera
// </h> !Peripheral drive

// <h> external storage
// ====================
// <i> Select the external storage media you use

// <e> EEPROM
#ifndef BSP_EEPROM_ENABLED
#define BSP_EEPROM_ENABLED 0  
#endif
// </e>

// <e> W25QXX
#ifndef BSP_W25QXX_ENABLED
#define BSP_W25QXX_ENABLED 0
#endif
// </e>

// </h> !external storage

// <h> Communication Protocol
// ==========================
// <e> BSP I2C
#ifndef BSP_I2C_ENABLED
#define BSP_I2C_ENABLED 0
#endif
// </e>

// <e> BSP SPI
#ifndef BSP_SPI_ENABLED
#define BSP_SPI_ENABLED 0
#endif
// </e>

// <e> BSP SCCB
#ifndef BSP_SCCB_ENABLED
#define BSP_SCCB_ENABLED 0
#endif
// </e>

// <e> BSP USART
#ifndef BSP_USART_ENABLED
#define BSP_USART_ENABLED 1
#endif
// </e>

// <e> BSP RS485
#ifndef BSP_RS485_ENABLED
#define BSP_RS485_ENABLED 0
#endif
// </e> 

// <e> BSP QUADSPI
#ifndef BSP_QUADSPI_ENABLED
#define BSP_QUADSPI_ENABLED 0
#endif
// </e>
// </h> !Communication Protocol

// <h> Custom algorithm
// ====================
// <e> CODING
#ifndef BSP_CODING_ENABLED
#define BSP_CODING_ENABLED 0
#endif
// </e>

// <e> DEDING
#ifndef BSP_DEDING_ENABLED
#define BSP_DEDING_ENABLED 0
#endif
// </e>

// <e> FILTER
#ifndef BSP_FILTER_ENABLED
#define BSP_FILTER_ENABLED 0
#endif
// </e>

// <e> BSP MATH
#ifndef BSP_MATH_ENABLED
#define BSP_MATH_ENABLED 0
#endif
// </e>

// <e> FFT
#ifndef BSP_FFT_ENABLED
#define BSP_FFT_ENABLED 0
#endif
// </e>

// <e> SPWM
#ifndef BSP_SPWM_ENABLED 
#define BSP_SPWM_ENABLED 0
#endif
// </e>
// </h> !Custom algorithm
// <<< end of configuration section >>>

/*!======= BSP Files Include =======!*/

#ifdef MCUID_STM32F1
#include "stm32f1xx_hal.h"
#endif /* !MCUID_STM32F1 */

#ifdef MCUID_STM32F4
#include "stm32f4xx_hal.h"
#endif /* !MCUID_STM32F4 */

#ifdef MCUID_STM32H7
#include "stm32h7xx_hal.h"
#endif /* !MCUID_STM32H7 */

#ifdef MCUID_STM32G4
#include "stm32g4xx_hal.h"
#endif /* !MCUID_STM32G4 */

#if RTOS_RTTHREAD_ENABLED
#include "rtthread.h"
#endif /* !RTOS_RTTHREAD_ENABLED */

#if RTOS_FREERTOS_ENABLED
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#if configUSE_QUEUE_SETS
#include "queue.h"
#endif /* configUSE_QUEUE_SETS */

#if (configUSE_MUTEXES || \
     configUSE_BINARY_SEMAPHORES || \
     configUSE_RECURSIVE_MUTEXES || \
     configUSE_COUNTING_SEMAPHORES)
#include "semphr.h"
#endif /* configUSE_SEMAPHORES */

#if configUSE_TIMERS
#include "timers.h"
#endif /* configUSE_TIMERS */

#if configGENERATE_RUN_TIME_STATS
#include "bsp_timbase.h"
#endif /* configGENERATE_RUN_TIME_STATS */
#endif /* !RTOS_FREERTOS_ENABLED */

#if BSP_CITE_MAIN
#include "main.h"
#endif /* !MAIN */

#if BSP_C_STD_STDIO || BSP_USART_ENABLED
#include <stdio.h>
#endif /* !STDIO */

#if BSP_C_STD_STDLIB
#include <stdlib.h>
#endif /* !STDLIB */

#if BSP_C_STD_STRING
#include <string.h>
#endif /* !STRING */

#if BSP_C_STD_STDINT
#include <stdint.h>
#endif /* !STDINT */

#if BSP_C_STD_MATH
#include <math.h>
#endif /* !MATH */

#if BSP_C_STD_LIMITS
#include <limits.h>
#endif /* !LIMITS */

#if BSP_SYS_ENABLED || BSP_I2C_ENABLED || BSP_INA226_ENABLED
#include "bsp_sys.h"
#endif /* !BSP_SYS_ENABLED */

#if BSP_COMMON_ENABLED
#include "bsp_common.h"
#endif /* !BSP_COMMON_ENABLED */

#if BSP_DELAY_ENABLED
#include "bsp_delay.h"
#endif /* !BSP_DELAY_ENABLED */

#if BSP_ADC_ENABLED
#include "bsp_adc.h"
#endif /* !BSP_ADC_ENABLED */

#if BSP_DAC_ENABLED
#include "bsp_dac.h"
#endif /* !BSP_DAC_ENABLED */

#if BSP_TIM_ENABLED
#include "bsp_tim.h"
#endif /* !BSP_TIM_ENABLED */

#if BSP_DWT_ENABLED
#include "bsp_dwt.h"
#endif /* !BSP_DWT_ENABLED */

#if BSP_LED_ENABLED
#include "bsp_led.h"
#endif /* !BSP_LED_ENABLED */

#if BSP_BUZZER_ENABLED
#include "bsp_buzzer.h"
#endif /* !BSP_BUZZER_ENABLED */

#if BSP_KEY_ENABLED
#include "bsp_key.h"
#endif /* !BSP_KEY_ENABLED */

#if BSP_AD9833_ENABLED
#include "bsp_ad9833.h"
#endif /*! BSP_AD9833_ENABLED */

#if BSP_AD9851_ENABLED
#include "bsp_ad9851.h"
#endif /* !BSP_AD9851_ENABLED */

#if BSP_AD9854_ENABLED
#include "bsp_ad9854.h"
#endif /* !BSP_AD9854_ENABLED */

#if BSP_AD9959_ENABLED
#include "bsp_ad9959.h"
#endif /* !__BSP_AD9959_ENABLED */

#if BSP_AD7606_ENABLED
#include "bsp_ad7606.h"
#endif /* !BSP_AD7606_ENABLED */

#if BSP_DAC8563_ENABLED
#include "bsp_dac8563.h"
#endif /* !BSP_DAC8563_ENABLED */

#if BSP_ADF4351_ENABLED
#include "bsp_adf4351.h"
#endif /* !BSP_ADF4351_ENABLED */

#if BSP_ADF4002_ENABLED
#include "bsp_adf4002.h"
#endif /* !BSP_ADF4002_ENABLED */

#if BSP_LMK61E07_ENABLED
#include "bsp_lmk61e07.h"
#endif /* !BSP_LMK61E07_ENABLED */

#if BSP_PE4302_ENABLED
#include "bsp_pe4302.h"
#endif /* !BSP_PE4302_ENABLED */

#if BSP_OLEDI2C_ENABLED
#include "bsp_oled_i2c.h"
#endif /* !BSP_OLEDI2C_ENABLED */

#if BSP_OLEDSPI_ENABLED
#include "bsp_oled_spi.h"
#endif /* !BSP_OLEDI2C_ENABLED */

#if BSP_LCD1602_ENABLED
#include "bsp_lcd1602.h"
#endif /* !BSP_LCD1602_ENABLED */

#if BSP_LCDTFT_ENABLED
#include "bsp_lcd_tft.h"
#endif /* !BSP_LCD_ENABLED */

#if BSP_UARTHMI_ENABLED
#include "bsp_usart_hmi.h"
#endif /* !BSP_HMI_ENABLED */

#if BSP_DS18B20_ENABLED
#include "bsp_ds18b20.h"
#endif /* !__BSP_DS18B20_ENABLED */

#if BSP_DHT11_ENABLED
#include "bsp_dht11.h"
#endif /* !BSP_DHT11_ENABLED */

#if BSP_DHT20_ENABLED
#include "bsp_dht20.h"
#endif /* !BSP_DHT20_ENABLED */

#if BSP_BMP280_ENABLED
#include "bsp_bmp280.h"
#endif /* !BSP_BMP280_ENABLED */

#if BSP_S12SD_ENABLED
#include "bsp_s12sd.h"
#endif /* !__BSP_S12SD_ENABLED */

#if BSP_GP2Y_ENABLED
#include "bsp_gp2y.h"
#endif /* !BSP_GP2Y_ENABLED */

#if BSP_GPS_ENABLED
#include "bsp_gps.h"
#endif /* !BSP_GPS_ENABLED */

#if BSP_WTU404F4_ENABLED
#include "bsp_wtu404f4.h"
#endif /* !BSP_WTU404F4_ENABLED */

#if BSP_VMSWIND_ENABLED
#include "bsp_vms_windsensor.h"
#endif /* !BSP_VMSWIND_ENABLED */

#if BSP_INA226_ENABLED
#include "bsp_ina226.h"
#endif /* !BSP_INA226_ENABLED */

#if BSP_TMI8260_ENABLED
#include "bsp_tmi8260.h"
#endif /* !__BSP_TMI8260_ENABLED */

#if BSP_OV2640_ENABLED
#include "bsp_ov2640.h"
#endif /* !BSP_OV2640_ENABLED */

#if BSP_EEPROM_ENABLED
#include "bsp_eeprom.h"
#endif /* !BSP_EEPROM_ENABLED */

#if BSP_W25QXX_ENABLED
#include "bsp_w25qxx.h"
#endif /* !BSP_W25QXX_ENABLED */

#if BSP_I2C_ENABLED || BSP_INA226_ENABLED
#include "bsp_i2c.h"
#endif /* !BSP_I2C_ENABLED */

#if BSP_SPI_ENABLED
#include "bsp_spi.h"
#endif /* !__BSP_I2C_ENABLED */

#if BSP_SCCB_ENABLED
#include "bsp_sccb.h"
#endif /* !BSP_SCCB_ENABLED */

#if BSP_USART_ENABLED
#include "bsp_usart.h"
#endif /* !BSP_USART_ENABLED */

#if BSP_RS485_ENABLED
#include "bsp_rs485.h"
#endif /* !BSP_RS485_ENABLED */

#if BSP_QUADSPI_ENABLED
#include "bsp_quadspi.h"
#endif /* !BSP_QUADSPI_ENABLED */

#if BSP_CODING_ENABLED
#include "bsp_encode.h"
#endif /*! BSP_CODING_ENABLED */

#if BSP_DECODE_ENABLED
#include "bsp_decode.h"
#endif /* !BSP_DECODE_ENABLED */

#if BSP_FILTER_ENABLED
#include "bsp_filter.h"
#endif /* !BSP_FILTER_ENABLED */

#if BSP_MATH_ENABLED
#include "bsp_math.h"
#endif /* !BSP_MATH_ENABLED */

#if BSP_FFT_ENABLED
#include "bsp_fft.h"
#endif /* !BSP_FFT_ENABLED */

#if BSP_SPWM_ENABLED
#include "bsp_spwm.h"
#endif /* !BSP_SPWM_ENABLED */

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_CONFIG__ */
