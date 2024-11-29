/**
 * @file bsp_usart.h
 * @author July (Email: JulyCub@163.com)
 * @brief USART Function
 * @version 0.1
 * @date 2022.06.25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_USART_H__
#define __BSP_USART_H__
#ifdef __cplusplus
extern "C" {
#endif

#if !__RTOS_RTTHREAD_ENABLED
/* Include BEGIN */
#include "bsp_config.h"
#include "stdio.h"
/* Include END */

/* Select the function BEGIN */
/*
 * @attention
 *
 * The same configuration is available in file "bsp_config.h".
 * This is a backup option. The Users are advised to complete the configuration
 * in file "bsp_config.h".
 */
/* -@- */
#if 0
#define __BSP_USART_Receive
#if 1
#define __BSP_USART_VariableReceive
#endif
#endif
#if 1
#define __BSP_USART_Transmit 1
#if 1
#define __BSP_USE_PRINTF
#endif /* ！__BSP_USE_PRINTF */
#endif /* ！__BSP_USART_Transmit */
/* Select the function END */

typedef enum
{
	USART_ERROR = 0,
	USART_OK = 1
} BSP_UsartState;

#ifdef __BSP_USART_Receive
#ifdef __BSP_USART_VariableReceive

#define USART_RX_LEN 200
#define USART_HANDLE huart1
#define USART_DMA_HANDLE hdma_usart1_rx

extern volatile uint8_t rx_len;
extern volatile uint8_t recv_end_flag;
extern uint8_t rx_buffer[USART_RX_LEN];

extern UART_HandleTypeDef USART_HANDLE;
extern DMA_HandleTypeDef USART_DMA_HANDLE;

#if __RTOS_FREERTOS_ENABLED
extern SemaphoreHandle_t USART_BinarySem_Handle;
#endif /* __RTOS_FREERTOS_ENABLED */

BSP_UsartState BSP_UsartVar_ExtraIRQHandler(void);
BSP_UsartState BSP_UsartVar_Conduct(void);
void BSP_UsartVar_Callback(uint8_t *str);

/**
 * @brief Call this function to start serial port reception
 * @note If FreeRTOS is enabled, a binary semaphore will be created.
 */
#if __RTOS_FREERTOS_ENABLED
#define __BSP_USART_DMA_RECEIVE_START() {                         \
	__HAL_UART_ENABLE_IT(&USART_HANDLE, UART_IT_IDLE);            \
	HAL_UART_Receive_DMA(&USART_HANDLE, rx_buffer, USART_RX_LEN); \
	USART_BinarySem_Handle = xSemaphoreCreateBinary();			  \
}
#else
#define __BSP_USART_DMA_RECEIVE_START() {                         \
	__HAL_UART_ENABLE_IT(&USART_HANDLE, UART_IT_IDLE);            \
	HAL_UART_Receive_DMA(&USART_HANDLE, rx_buffer, USART_RX_LEN); \
}

#endif /* __RTOS_FREERTOS_ENABLED */
#endif /* __BSP_USART_VariableReceive */
#endif /* __BSP_USART_Receive */

#if __BSP_USART_Transmit >= 1
#define USART_HANDLE_PRF1 huart1
#if __BSP_USART_Transmit >= 2
#define USART_HANDLE_PRF2 huart2
#if __BSP_USART_Transmit >= 3
#define USART_HANDLE_PRF3 huart3
#endif
#endif

int fputc(int ch, FILE *f);
void bsprif(UART_HandleTypeDef *huart, char *fmt, ...);
void bsprif1(char *fmt, ...);
void bsprif2(char *fmt, ...);
void bsprif3(char *fmt, ...);
#endif /* __BSP_USART_Transmit */
/* Function declaration END */

#endif

#ifdef __cplusplus
}
#endif
#endif /* __BSP_USART_H__ */
