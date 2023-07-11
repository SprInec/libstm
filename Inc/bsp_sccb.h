/**
 * @file bsp_sccb.h
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023.05.11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_SCCB_
#define __BSP_SCCB_

#include "bsp_config.h"

#ifdef __BSP_STM32F1_ENABLED
#include "stm32f1xx_hal_gpio.h"
#endif
#ifdef __BSP_STM32F4_ENABLED
#include "stm32f4xx_hal_gpio.h"
#endif
#ifdef __BSP_STM32G4_ENABLED
#include "stm32g4xx_hal_gpio.h"
#endif
#ifdef __BSP_STM32H7_ENABLED
#include "stm32h7xx_hal_gpio.h"
#endif

#define BSP_SCCB_PORT GPIOD
#define BSP_SCCB_SCL GPIO_PIN_6
#define BSP_SCCB_SDA GPIO_PIN_7

#define __BSP_RCC_GPIOx_CLK_ENABLE(__GPIOx__) __HAL_RCC_##__GPIOx__##_CLK_ENABLE()

#define SCCB_SDA_IN()                            \
    {                                            \
        BSP_SCCB_PORT->MODER &= ~(3 << (7 * 2)); \
        BSP_SCCB_PORT->MODER |= (0 << (7 * 2));  \
    }

#define SCCB_SDA_OUT()                           \
    {                                            \
        BSP_SCCB_PORT->MODER &= ~(3 << (7 * 2)); \
        BSP_SCCB_PORT->MODER |= (1 << (7 * 2));  \
    }

#ifndef __BSP_STM32H7_ENABLED
#define SCCB_SCL PDout(6)
#define SCCB_SDA PDout(7)
#define SCCB_READ_SDA PDin(7)
#else
#define SCCB_SCL(__STATE__) HAL_GPIO_WritePin(BSP_SCCB_PORT, BSP_SCCB_SCL, __STATE__)
#define SCCB_SDA(__STATE__) HAL_GPIO_WritePin(BSP_SCCB_PORT, BSP_SCCB_SDA, __STATE__)
#define SCCB_READ_SDA HAL_GPIO_ReadPin(BSP_SCCB_PORT, BSP_SCCB_SDA)
#endif
#define SCCB_ID 0x60 // OV2640的ID

void BSP_SCCB_Init(void);
uint8_t BSP_SCCB_WriteRegister(uint8_t reg, uint8_t data);
uint8_t BSP_SCCB_ReadRegister(uint8_t reg);

#endif
