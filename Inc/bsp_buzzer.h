/**
 ******************************************************************************
 * @file		:bsp_buzzer.h
 * @brief		:The board support package for buzzer.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.26
 ******************************************************************************
 */

#ifndef __BSP_BUZZER_H__
#define __BSP_BUZZER_H__

#include "bsp_config.h"
#include "gpio.h"

#define BSP_BUZZER_Port			GPIOA
#define BSP_BUZZER_Pin			GPIO_PIN_1

#define BSP_BUZZER_Ring			({	HAL_GPIO_WritePin(BSP_BUZZER_Port, BSP_BUZZER_PIN, GPIO_PIN_SET);\
									HAL_Delay(500);\
									HAL_GPIO_WritePin(BSP_BUZZER_Port, BSP_BUZZER_PIN, GPIO_PIN_RESET);})

#endif
