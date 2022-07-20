/**
 ******************************************************************************
 * @file		:bsp_led.h
 * @brief		:The board support package for led.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.26
 ******************************************************************************
 */

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "bsp_config.h"
#include "gpio.h"

#define BSP_LED_Port1			GPIOA
#define BSP_LED_Port2			GPIOD

#define BSP_LED_PIN1			GPIO_PIN_8
#define BSP_LED_PIN2			GPIO_PIN_2

#define __BSP_LED1_Toggle						HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1)
#define __BSP_LED2_Toggle						HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2)

#define __BSP_LED1_Ficker(n)		({	HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1);\
																		HAL_Delay(n);\
																		HAL_GPIO_TogglePin(BSP_LED_Port1, BSP_LED_PIN1);\
																		HAL_Delay(n); })

#define __BSP_LED2_Ficker(n)		({	HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2);\
																		HAL_Delay(n);\
																		HAL_GPIO_TogglePin(BSP_LED_Port2, BSP_LED_PIN2);\
																		HAL_Delay(n); })


void bsp_led_Init(void);

#endif /* __BSP_LED_H__ */
																		
																		
																		
																		
																		
																		
