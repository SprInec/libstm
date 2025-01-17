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

#ifdef BSP_MCU_USER_DEFINED
#define BSP_LED_Port0 GPIOA
#define BSP_LED_Port1 GPIOC
#define BSP_LED_Port2 GPIOD

#define BSP_LED_PIN0 GPIO_PIN_15
#define BSP_LED_PIN1 GPIO_PIN_12
#define BSP_LED_PIN2 GPIO_PIN_0

#define BSP_LED_PIN_ON  GPIO_PIN_SET
#define BSP_LED_PIN_OFF GPIO_PIN_RESET

#define __BSP_RCC_LED_CLK_ENABLE() { \
	__HAL_RCC_GPIOA_CLK_ENABLE();    \
	__HAL_RCC_GPIOC_CLK_ENABLE();    \
	__HAL_RCC_GPIOD_CLK_ENABLE();    \
}

#define __BSP_LED0_ON() HAL_GPIO_WritePin(BSP_LED_Port0, BSP_LED_PIN0, BSP_LED_PIN_ON)
#define __BSP_LED1_ON() HAL_GPIO_WritePin(BSP_LED_Port1, BSP_LED_PIN1, BSP_LED_PIN_ON)
#define __BSP_LED2_ON() HAL_GPIO_WritePin(BSP_LED_Port2, BSP_LED_PIN2, BSP_LED_PIN_ON)

#define __BSP_LED0_OFF() HAL_GPIO_WritePin(BSP_LED_Port0, BSP_LED_PIN0, BSP_LED_PIN_OFF)
#define __BSP_LED1_OFF() HAL_GPIO_WritePin(BSP_LED_Port1, BSP_LED_PIN1, BSP_LED_PIN_OFF)
#define __BSP_LED2_OFF() HAL_GPIO_WritePin(BSP_LED_Port2, BSP_LED_PIN2, BSP_LED_PIN_OFF)

#define __BSP_LED0_TOGGLE() HAL_GPIO_TogglePin(BSP_LED_Port0, BSP_LED_PIN0)
#define __BSP_LED1_TOGGLE() HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1)
#define __BSP_LED2_TOGGLE() HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2)

#define __BSP_LED0_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED0_TOGGLE();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED0_TOGGLE();        \
		vTaskDelay(__TIME__);       \
	} while (0)

#define __BSP_LED1_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_TOGGLE();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED1_TOGGLE();        \
		vTaskDelay(__TIME__);       \
	} while (0)

#define __BSP_LED2_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_TOGGLE();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED2_TOGGLE();        \
		vTaskDelay(__TIME__);       \
	} while (0)
#endif /* BSP_MCU_USER_DEFINED */

#ifdef BSP_MCU_DEVEBOX_STM32F407VET6
#define BSP_LED_Port1 GPIOF
#define BSP_LED_Port2 GPIOF

#define BSP_LED_PIN1 GPIO_PIN_9
#define BSP_LED_PIN2 GPIO_PIN_10

#define BSP_LED_PIN_ON  GPIO_PIN_RESET
#define BSP_LED_PIN_OFF GPIO_PIN_SET

#define __BSP_LED1_ON() HAL_GPIO_WritePin(BSP_LED_Port1, BSP_LED_PIN1, BSP_LED_PIN_ON)
#define __BSP_LED2_ON() HAL_GPIO_WritePin(BSP_LED_Port2, BSP_LED_PIN2, BSP_LED_PIN_ON)

#define __BSP_LED1_OFF() HAL_GPIO_WritePin(BSP_LED_Port1, BSP_LED_PIN1, BSP_LED_PIN_OFF)
#define __BSP_LED2_OFF() HAL_GPIO_WritePin(BSP_LED_Port2, BSP_LED_PIN2, BSP_LED_PIN_OFF)

#define __BSP_LED1_TOGGLE() HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1)
#define __BSP_LED2_TOGGLE() HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2)

#if RTOS_RTTHREAD_ENABLED
#define __BSP_LED1_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED1_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)

#define __BSP_LED2_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED2_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)
#elif RTOS_FREERTOS_ENABLED
#define __BSP_LED1_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_TOGGLE();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED1_TOGGLE();        \
		vTaskDelay(__TIME__);       \
	} while (0)

#define __BSP_LED2_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_TOGGLE();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED2_TOGGLE();        \
		vTaskDelay(__TIME__);       \
	} while (0)
#else
#define __BSP_LED1_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_TOGGLE();        \
		delay_ms(__TIME__);         \
		__BSP_LED1_TOGGLE();        \
		delay_ms(__TIME__);         \
	} while (0)

#define __BSP_LED2_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_TOGGLE();        \
		delay_ms(__TIME__);         \
		__BSP_LED2_TOGGLE();        \
		delay_ms(__TIME__);         \
	} while (0)
#endif
#endif /* !BSP_MCU_DEVEBOX_STM32F407VET6 */

#ifdef BSP_MCU_NUCLEO_H7A3ZIQ
#define BSP_LED_Port1 GPIOB
#define BSP_LED_Port2 GPIOE
#define BSP_LED_Port3 GPIOB

#define BSP_LED_PIN1 GPIO_PIN_0
#define BSP_LED_PIN2 GPIO_PIN_1
#define BSP_LED_PIN3 GPIO_PIN_14

#define __BSP_LED1_TOGGLE() HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1)
#define __BSP_LED2_TOGGLE() HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2)
#define __BSP_LED3_TOGGLE() HAL_GPIO_TogglePin(BSP_LED_Port3, BSP_LED_PIN3)

/* Macro Function */
#if RTOS_RTTHREAD_ENABLED
#define __BSP_LED1_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED1_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)

#define __BSP_LED2_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED2_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)

#define __BSP_LED3_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED3_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED3_TOGGLE();        \
		rt_thread_mdelay(__TIME__); \
	} while (0)
#elif RTOS_FREERTOS_ENABLED
#define __BSP_LED1_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_TOGGLE();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED1_TOGGLE();        \
		vTaskDelay(__TIME__);       \
	} while (0)

#define __BSP_LED2_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_TOGGLE();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED2_TOGGLE();        \
		vTaskDelay(__TIME__);       \
	} while (0)

#define __BSP_LED3_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED3_TOGGLE();        \
		vTaskDelay(__TIME__);       \
		__BSP_LED3_TOGGLE();        \
		vTaskDelay(__TIME__);       \
	} while (0)
#else
#define __BSP_LED1_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED1_TOGGLE();        \
		delay_ms(__TIME__);         \
		__BSP_LED1_TOGGLE();        \
		delay_ms(__TIME__);         \
	} while (0)

#define __BSP_LED2_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED2_TOGGLE();        \
		delay_ms(__TIME__);         \
		__BSP_LED2_TOGGLE();        \
		delay_ms(__TIME__);         \
	} while (0)

#define __BSP_LED3_FICKER(__TIME__) \
	do                              \
	{                               \
		__BSP_LED3_TOGGLE();        \
		delay_ms(__TIME__);         \
		__BSP_LED3_TOGGLE();        \
		delay_ms(__TIME__);         \
	} while (0)
#endif
#endif /* !BSP_MCU_NUCLEO_H7A3ZIQ */

#ifdef BSP_MCU_DEVEBOX_STM32H743VIT6
#define BSP_LED_Port GPIOA
#define BSP_LED_PIN GPIO_PIN_1

#define __BSP_LED_TOGGLE() HAL_GPIO_TogglePin(BSP_LED_Port, BSP_LED_PIN)
#if RTOS_RTTHREAD_ENABLED
#define __BSP_LED_FICKER(__TIME__)  \
	do                              \
	{                               \
		__BSP_LED_TOGGLE();         \
		rt_thread_mdelay(__TIME__); \
		__BSP_LED_TOGGLE();         \
		rt_thread_mdelay(__TIME__); \
	} while (0)
#elif RTOS_FREERTOS_ENABLED
#define __BSP_LED_FICKER(__TIME__) \
	do                             \
	{                              \
		__BSP_LED_TOGGLE();        \
		vTaskDelay(__TIME__);      \
		__BSP_LED_TOGGLE();        \
		vTaskDelay(__TIME__);      \
	} while (0)
#else
#define __BSP_LED_FICKER(__TIME__) \
	do                             \
	{                              \
		__BSP_LED_TOGGLE();        \
		delay_ms(__TIME__);        \
		__BSP_LED_TOGGLE();        \
		delay_ms(__TIME__);        \
	} while (0)
#endif
#endif /* !BSP_MCU_DEVEBOX_STM32H743VIT6 */

void BSP_LED_Init(void);

#ifdef __cplusplus
}
#endif /* !__cplusplus */
#endif /* !__BSP_LED_H__ */
