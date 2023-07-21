/**
 * @file bsp_usart.c
 * @author July (Email: JulyCub@163.com)
 * @brief The support for UART devices
 * @version 0.1
 * @date 2022.06.25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bsp_usart.h"
#include "usart.h"
#include "stdarg.h"

#if !__RTOS_RTTHREAD_ENABLED
#ifdef __BSP_USART_Receive

/* Variable declarations BEGIN */
#ifdef __BSP_USART_VariableReceive
volatile uint8_t rx_len = 0;
volatile uint8_t recv_end_flag = 0;
uint8_t rx_buffer[USART_RX_LEN];

extern DMA_HandleTypeDef USART_DMA_HANDLE;
#endif
/* Variable declarations END */

/**
 * @brief	Serial port interrupt callback function.
 */
/*
__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

}
*/

#ifdef __BSP_USART_VariableReceive
/**
 * @brief	USART variable length receiving additional initialization content.
 * @attention
 *
 * This function need the users to call it at function "USARTX_IRQHandler()"
 * in the file <stm32f1xx_it.c> or <stm32f4xx_it.c>.
 */
BSP_UsartState BSP_UsartVar_ExtraIRQHandler(void)
{
	uint32_t tmp_flag = 0;
	uint32_t temp;

	tmp_flag = __HAL_UART_GET_FLAG(&USART_HANDLE, UART_FLAG_IDLE);
	if ((tmp_flag != RESET))
	{
		__HAL_UART_CLEAR_IDLEFLAG(&USART_HANDLE);
		HAL_UART_DMAStop(&USART_HANDLE);
#ifdef __BSP_MCU_DEVEBOX_STM32F103C6T6
		temp = USART_DMA_HANDLE.Instance->CNDTR;
#endif
#ifdef __BSP_MCU_DEVEBOX_STM32F407VET6
		temp = __HAL_DMA_GET_COUNTER(&USART_DMA_HANDLE);
#endif									
		rx_len = USART_RX_LEN - temp;
		recv_end_flag = 1;
	}
	return USART_OK;
}

/**
 * @brief	Users need to manually configure the code.
 * @attention
 *
 * 	The users need to paste the following two functions segments between ------
 * " USER CODE BEGIN USARTX_Init 2 " and " USER CODE END USARTX_Init 2 " of the
 * " MX_USART1_UART_Init(void) "in the <usart.c>.
 *
 * functions segments:
 *
 *	-> 	__HAL_UART_ENABLE_IT(&USART_HANDLE, UART_IT_IDLE);
 * 	->	HAL_UART_Receive_DMA(&USART_HANDLE, rx_buffer, USART_RX_LEN);
 *
 */

/**
 *@brief	USART variable length receiving execution function.
 */
BSP_UsartState BSP_UsartVar_Conduct(void)
{
	_Bool uart_state = 0;
	if (recv_end_flag == 1)
	{
#if 1
		/* 串口接收指示灯 */
		__BSP_LED1_Ficker(50);
#endif
#if 0
		/* 返回串口接收到的数据 */
		bsprif1("%s\n", rx_buffer);
#endif
#if 1
		/* 用户自定义串口回调 */
		BSP_UsartVar_Callback(rx_buffer);
#endif
		for (uint8_t i = 0; i < rx_len; i++)
		{
			rx_buffer[i] = 0;
		}
		rx_len = 0;
		recv_end_flag = 0;
		uart_state = USART_OK;
	}
	HAL_UART_Receive_DMA(&USART_HANDLE, rx_buffer, USART_RX_LEN);

	return uart_state;
}

/**
 *@brief	The user redefines this function to handle the information received
 *				by the serial port.
 */
__weak void BSP_UsartVar_Callback(uint8_t *str)
{
	/* User Code */
}

#endif /* __BSP_USART_VariableReceive */

#endif /* __BSP_USART_Receive */

#if __BSP_USART_Transmit >= 1

/**
 * @brief	Redirect the output function.
 */
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&USART_HANDLE_PRF1, (uint8_t *)&ch, 1, 0xffff);
	return ch;
}

#ifdef __BSP_USE_PRINTF
/* bsp printf 1 */
void bsprif1(char *fmt, ...)
{
	va_list va;
	int i = 0;
	char buffer[256];

	va_start(va, fmt);
	i = vsprintf(buffer, fmt, va);
	va_end(va);
	HAL_UART_Transmit(&USART_HANDLE_PRF1, (uint8_t *)buffer, i, 0XFF);
}
#if __BSP_USART_Transmit >= 2
/* bsp printf 2 */
void bsprif2(char *fmt, ...)
{
	va_list va;
	int i = 0;
	char buffer[256];

	va_start(va, fmt);
	i = vsprintf(buffer, fmt, va);
	va_end(va);
	HAL_UART_Transmit(&USART_HANDLE_PRF2, (uint8_t *)buffer, i, 0XFF);
}
#if __BSP_USART_Transmit >= 3
/* bsp printf 3 */
void bsprif3(char *fmt, ...)
{
	va_list va;
	int i = 0;
	char buffer[256];

	va_start(va, fmt);
	i = vsprintf(buffer, fmt, va);
	va_end(va);
	HAL_UART_Transmit(&USART_HANDLE_PRF3, (uint8_t *)buffer, i, 0XFF);
}
#endif
#endif
#endif
#endif /* __BSP_USART_Transmit */
#endif
