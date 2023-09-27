/**
 * @file bsp_user.c
 * @author July (Email: JulyCub@163.com)
 * @brief User Functions
 * @version 0.1
 * @date 2023.08.19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_user.h"
#include "string.h"
#include "usart.h"

uint8_t timeflag = 0;
uint8_t datetime[20] = {0};

void BSP_UsartVar_Callback(uint8_t *str)
{
    sprintf((char *)datetime, "%s", str);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{   
    if (huart->Instance == UART4)
    {
        timeflag = 1;
        HAL_UART_Receive_IT(&huart4, (uint8_t *)datetime, 19);
    }
}