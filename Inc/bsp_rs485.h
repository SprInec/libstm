/**
 * @file rs485.h
 * @author July
 * @brief RS485 Driver
 * @version 0.1
 * @date 2023.05.22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_RS485_H__
#define __BSP_RS485_H__

#include "bsp_config.h"
#include "gpio.h"

#define RS485_UART_HANDLE huart3
#define RS485_RE_PORT GPIOB
#define RS485_DE_PORT GPIOB
#define RS485_RE_PIN GPIO_PIN_12
#define RS485_DE_PIN GPIO_PIN_12

typedef enum
{
    RS485_SUCCESS = 0,
    RS485_ERROR
} RS485_StateTypeDef;

#define RE_HIGH() HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_SET)
#define RE_LOW() HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_RESET)
#define DE_HIGH() HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_SET)
#define DE_LOW() HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_RESET)

#define RS485_SendByte(_ch_) HAL_UART_Transmit(&RS485_UART_HANDLE, (uint8_t *)(_ch_), 1, 0xFFFF)
#define RS485_ReadByte(_ch_) HAL_UART_Receive(&RS485_UART_HANDLE, (uint8_t *)(_ch_), 1, 0xFF)

void BSP_RS485_Init(void);
RS485_StateTypeDef RS485_SendData(const uint8_t *pData, uint8_t len);
RS485_StateTypeDef RS485_ReadData(uint8_t *pData, uint8_t len);

#endif /* __BSP_RS485_H__ */