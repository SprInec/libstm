/*
 ******************************************************************************
 * @file		:bsp_usart.c
 * @brief		:The board support package for USART.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.25
 ******************************************************************************
 */

#include "bsp_usart.h"
#include "usart.h"
#include "stdarg.h"

#ifdef __BSP_USART_Receive

/* Variable declarations BEGIN */
#ifdef __BSP_USART_VariableReceive
volatile uint8_t rx_len = 0;
volatile uint8_t recv_end_flag = 0;
uint8_t rx_buffer[USART_RX_LEN];

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
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
BSP_UsartState bsp_usartVar_ExtraIRQHandler(void)
{
	uint32_t tmp_flag = 0;
	uint32_t temp;

	tmp_flag = __HAL_UART_GET_FLAG(&USART_HANDLE, UART_FLAG_IDLE);
    if((tmp_flag != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&USART_HANDLE); 
        // temp = USART_HANDLE.Instance->SR;
        // temp = USART_HANDLE.Instance->DR;
        HAL_UART_DMAStop(&USART_HANDLE); // 与上两句等效
				/* !< The user needs to declare variable "DMA_HandleTypeDef hdma_usart1_rx"
							outside of file <usart.h> */
				#ifdef __BSP_STM32F1_ENABLED
        		temp  = hdma_usart2_rx.Instance->CNDTR;
				#endif /* __BSP_STM32F1 */
				#ifdef __BSP_STM32F4_ENABLED
				// temp  = hdma_usart2_rx.Instance->NDTR;
				temp = __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);	// 与上一句等效
#endif /* __BSP_STM32F4 */
        rx_len =  USART_RX_LEN - temp;
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
BSP_UsartState bsp_usartVar_Conduct(void)
{
	_Bool uart_state = 0;
	if(recv_end_flag == 1)
	{
#if 1
		/* 串口接收指示灯 */
	 	__BSP_LED1_Ficker(50);
#endif
#if 1 
		/* 返回串口接收到的数据 */
		bsprif1("%sx\n", rx_buffer);
#endif
#if 1
		/* 用户自定义串口回调 */
		bsp_usartVar_Callback(rx_buffer);
#endif
		for(uint8_t i=0;i<rx_len;i++)
		{
			rx_buffer[i]=0;
		}
		rx_len=0;
		recv_end_flag=0;
		uart_state = USART_OK;
	}
	HAL_UART_Receive_DMA(&USART_HANDLE, rx_buffer, USART_RX_LEN);
	return uart_state;
}

/**
 *@brief	The user redefines this function to handle the information received
 *				by the serial port.
 */
__weak void bsp_usartVar_Callback(uint8_t* str)
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

/* bsp printf 1 */
void bsprif1(char *fmt, ...)
{
	va_list va;
	int i = 0;
	char buffer[256];
	
	va_start(va, fmt);
	i = vsprintf(buffer, fmt, va);
	va_end(va);
	HAL_UART_Transmit(&USART_HANDLE_PRF1, (uint8_t *)buffer ,i ,0XFF);
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
	HAL_UART_Transmit(&USART_HANDLE_PRF2, (uint8_t *)buffer ,i ,0XFF);
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
	HAL_UART_Transmit(&USART_HANDLE_PRF3, (uint8_t *)buffer ,i ,0XFF);
}
#endif
#endif
#endif /* __BSP_USART_Transmit */
