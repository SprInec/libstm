/**
 * @file bsp_led.h
 * @author July (Email: JulyCub@163.com)
 * @brief The board support package for led.
 * @version 0.1
 * @date 2022.06.26
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "bsp_config.h"
#include "gpio.h"

#ifdef __BSP_STM32F4_ENABLED
#define BSP_LED_Port1 GPIOF
#define BSP_LED_Port2 GPIOF

#define BSP_LED_PIN1 GPIO_PIN_9
#define BSP_LED_PIN2 GPIO_PIN_10

#define __BSP_LED1_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1)
#define __BSP_LED2_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2)

#ifdef __RTOS_RTTHREAD_ENABLED
#define __BSP_LED1_Ficker(__TIME__) ({	__BSP_LED1_Toggle(); 		\
										rt_thread_mdelay(__TIME__); \
										__BSP_LED1_Toggle(); 		\
										rt_thread_mdelay(__TIME__); })

#define __BSP_LED2_Ficker(__TIME__) ({	__BSP_LED2_Toggle(); 		\
										rt_thread_mdelay(__TIME__);	\
										__BSP_LED2_Toggle(); 		\
										rt_thread_mdelay(__TIME__); })
#else
#define __BSP_LED1_Ficker(__TIME__) ({	__BSP_LED1_Toggle(); \
										delay_ms(__TIME__);  \
										__BSP_LED1_Toggle(); \
										delay_ms(__TIME__); })

#define __BSP_LED2_Ficker(__TIME__) ({	__BSP_LED2_Toggle(); \
										delay_ms(__TIME__);  \
										__BSP_LED2_Toggle(); \
										delay_ms(__TIME__); })
#endif
#endif

#ifdef __BSP_STM32H7_ENABLED
#define BSP_LED_Port1 GPIOB
#define BSP_LED_Port2 GPIOE
#define BSP_LED_Port3 GPIOB

#define BSP_LED_PIN1 GPIO_PIN_0
#define BSP_LED_PIN2 GPIO_PIN_1
#define BSP_LED_PIN3 GPIO_PIN_14

#define __BSP_LED1_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1)
#define __BSP_LED2_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2)
#define __BSP_LED3_Toggle() HAL_GPIO_TogglePin(BSP_LED_Port3, BSP_LED_PIN3)


/* Macor Function */
#ifdef __RTOS_RTTHREAD_ENABLED
#define __BSP_LED1_Ficker(__TIME__) ({	__BSP_LED1_Toggle(); 		\
										rt_thread_mdelay(__TIME__); \
										__BSP_LED1_Toggle(); 		\
										rt_thread_mdelay(__TIME__); })

#define __BSP_LED2_Ficker(__TIME__) ({	__BSP_LED2_Toggle(); 		\
										rt_thread_mdelay(__TIME__);	\
										__BSP_LED2_Toggle(); 		\
										rt_thread_mdelay(__TIME__); })

#define __BSP_LED3_Ficker(__TIME__) ({	__BSP_LED3_Toggle(); 		\
										rt_thread_mdelay(__TIME__); \
										__BSP_LED3_Toggle(); 		\
										rt_thread_mdelay(__TIME__); })
#else
#define __BSP_LED1_Ficker(__TIME__) ({	__BSP_LED1_Toggle(); \
										delay_ms(__TIME__);  \
										__BSP_LED1_Toggle(); \
										delay_ms(__TIME__); })

#define __BSP_LED2_Ficker(__TIME__) ({	__BSP_LED2_Toggle(); \
										delay_ms(__TIME__);  \
										__BSP_LED2_Toggle(); \
										delay_ms(__TIME__); })

#define __BSP_LED3_Ficker(__TIME__) ({	__BSP_LED3_Toggle(); \
										delay_ms(__TIME__);  \
										__BSP_LED3_Toggle(); \
										delay_ms(__TIME__); })
#endif
#endif

void BSP_LED_Init(void);

#endif /* __BSP_LED_H__ */
