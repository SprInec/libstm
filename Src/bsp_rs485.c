/**
 * @file rs485.c
 * @author July
 * @brief RS485 Driver
 * @version 0.1
 * @date 2023.05.22
 * @usecodeing GBK
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_rs485.h"
#include "usart.h"

/**
 * @brief RS485初始化
 */
void BSP_RS485_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_RESET);

    /*Configure GPIO pin : PA8 */
    GPIO_InitStruct.Pin = RS485_RE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RS485_RE_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RS485_DE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RS485_DE_PORT, &GPIO_InitStruct);
    MX_USART3_UART_Init();
}

/**
 * @brief RS485发送数据
 * @param pData 数据/指令
 * @param len 数据/指令长度
 * @return  RS485_SUCCESS 成功 |
 *          RS485_ERROR 失败
 */
RS485_StateTypeDef RS485_SendData(const uint8_t *pData, uint8_t len)
{
    DE_LOW();
    delay_us(1);
    DE_HIGH();

    for (uint16_t i = 0; i < len; i++)
        if (RS485_SendByte(pData + i))
            return RS485_ERROR;

    delay_us(1);
    DE_LOW();
    return RS485_SUCCESS;
}

/**
 * @brief  RS485接收数据
 * @param pData 接收数据地址
 * @param len 数据长度
 * @return  RS485_SUCCESS 成功 |
 *          RS485_ERROR 失败
 */
RS485_StateTypeDef RS485_ReadData(uint8_t *pData, uint8_t len)
{
    RE_HIGH();
    DE_HIGH();
    delay_us(1);
    RE_LOW();
    DE_LOW();

    for (uint16_t i = 0; i < len; i++)
        if (RS485_ReadByte(pData + i))
            return RS485_ERROR;

    delay_us(1);
    RE_HIGH();
    DE_HIGH();

    return RS485_SUCCESS;
}