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
#include "stm32h7xx_hal_gpio.h"

#define BSP_SCCB_PORT GPIOA
#define BSP_SCCP_SCL GPIO_PIN_3
#define BSP_SCCP_SDA GPIO_PIN_4

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

#define SCCB_SCL PDout(3)
#define SCCB_SDA PDout(4)

#define SCCB_READ_SDA PDin(4)
#define SCCB_ID 0x60 // OV2640的ID

uint8_t bsp_sccb_WriteRegister(uint8_t reg, uint8_t data);
uint8_t bsp_sccb_ReadRegister(uint8_t reg);

#endif
