/**
 * @file bsp_led.h
 * @author July (Email: JulyCub@163.com)
 * @brief The board support package for led.
 * @version v1.2
 * @date 2022.06.26
 * @updata 2023.07.15 - Add DevEBox-H743 LED support.
 *         2024.11.02 - Add FreeRTOS support. Optimized grammatical.
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_LED_H__
#define __BSP_LED_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"
#include "gpio.h"

#ifdef __BSP_MCU_DEVEBOX_STM32F407VET6
#define BSP_LED_Port1 GPIOF
#define BSP_LED_Port2 GPIOF

#define BSP_LED_PIN1 GPIO_PIN_9
#define BSP_LED_PIN2 GPIO_PIN_10

#define __BSP_LED1_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1)
#define __BSP_LED2_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2)

#if __RTOS_RTTHREAD_ENABLED
#define __BSP_LED1_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_Toggle();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED1_Toggle();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)

#define __BSP_LED2_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_Toggle();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED2_Toggle();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)
#elif __RTOS_FREERTOS_ENABLED
#define __BSP_LED1_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_Toggle();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED1_Toggle();        \
		vTaskDelay(__TIME__);       \
	} while (0)

#define __BSP_LED2_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_Toggle();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED2_Toggle();        \
		vTaskDelay(__TIME__);       \
	} while (0)
#else
#define __BSP_LED1_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_Toggle();        \
		delay_ms(__TIME__);         \
		__BSP_LED1_Toggle();        \
		delay_ms(__TIME__);         \
	} while (0)

#define __BSP_LED2_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_Toggle();        \
		delay_ms(__TIME__);         \
		__BSP_LED2_Toggle();        \
		delay_ms(__TIME__);         \
	} while (0)
#endif
#endif /* !__BSP_MCU_DEVEBOX_STM32F407VET6 */

#ifdef __BSP_MCU_NUCLEO_H7A3ZIQ
#define BSP_LED_Port1 GPIOB
#define BSP_LED_Port2 GPIOE
#define BSP_LED_Port3 GPIOB

#define BSP_LED_PIN1 GPIO_PIN_0
#define BSP_LED_PIN2 GPIO_PIN_1
#define BSP_LED_PIN3 GPIO_PIN_14

#define __BSP_LED1_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1)
#define __BSP_LED2_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2)
#define __BSP_LED3_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port3, BSP_LED_PIN3)

/* Macro Function */
#if __RTOS_RTTHREAD_ENABLED
#define __BSP_LED1_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_Toggle();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED1_Toggle();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)

#define __BSP_LED2_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_Toggle();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED2_Toggle();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)

#define __BSP_LED3_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED3_Toggle();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED3_Toggle();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)
#elif __RTOS_FREERTOS_ENABLED
#define __BSP_LED1_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_Toggle();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED1_Toggle();        \
		vTaskDelay(__TIME__);       \
	} while (0)

#define __BSP_LED2_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_Toggle();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED2_Toggle();        \
		vTaskDelay(__TIME__);       \
	} while (0)

#define __BSP_LED3_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED3_Toggle();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED3_Toggle();        \
		vTaskDelay(__TIME__);       \
	} while (0)
#else
#define __BSP_LED1_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_Toggle();        \
		delay_ms(__TIME__);         \
		__BSP_LED1_Toggle();        \
		delay_ms(__TIME__);         \
	} while (0)

#define __BSP_LED2_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_Toggle();        \
		delay_ms(__TIME__);         \
		__BSP_LED2_Toggle();        \
		delay_ms(__TIME__);         \
	} while (0)

#define __BSP_LED3_Ficker(__TIME__) \
	do                              \
	{                               \
		__BSP_LED3_Toggle();        \
		delay_ms(__TIME__);         \
		__BSP_LED3_Toggle();        \
		delay_ms(__TIME__);         \
	} while (0)
#endif
#endif /* !__BSP_MCU_NUCLEO_H7A3ZIQ */

#ifdef __BSP_MCU_DEVEBOX_STM32H743VIT6
#define BSP_LED_Port GPIOA
#define BSP_LED_PIN GPIO_PIN_1

#define __BSP_LED_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port, BSP_LED_PIN)
#if __RTOS_RTTHREAD_ENABLED
#define __BSP_LED_Ficker(__TIME__)  \
	do                              \
	{                               \
		__BSP_LED_Toggle();         \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED_Toggle();         \
		rt_thread_mdelay(__TIME__); \
	} while (0)
#elif __RTOS_FREERTOS_ENABLED
#define __BSP_LED_Ficker(__TIME__) \
	do                             \
	{                              \
		__BSP_LED_Toggle();        \
		vTaskDelay(__TIME__);      \
		__BSP_LED_Toggle();        \
		vTaskDelay(__TIME__);      \
	} while (0)
#else
#define __BSP_LED_Ficker(__TIME__) \
	do                             \
	{                              \
		__BSP_LED_Toggle();        \
		delay_ms(__TIME__);        \
		__BSP_LED_Toggle();        \
		delay_ms(__TIME__);        \
	} while (0)
#endif
#endif /* !__BSP_MCU_DEVEBOX_STM32H743VIT6 */

void BSP_LED_Init(void);

#ifdef __cplusplus
}
#endif /* !__cplusplus */
#endif /* !__BSP_LED_H__ */
