/**
 * @file bsp_dht11.c
 * @author fire
 * @brief DHT11 Hardware Support
 * @version 0.1
 * @date 2023.08.21
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_DHT11_H__
#define __BSP_DHT11_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

// #define DHT11_HIGH GPIO_PIN_SET
// #define DHT11_LOW GPIO_PIN_RESET

// #define Bit_RESET 0
// #define Bit_SET 1

// #define DHT11_PORT GPIOB
// #define DHT11_PIN GPIO_PIN_9
// #define DHT11_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

// #define DHT11_DATA_IN() HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)
// #define DHT11_DATA_OUT(value) HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, value)

// typedef struct DHT11_DATA
// {
//     uint8_t humi_int;  
//     uint8_t humi_deci; 
//     uint8_t temp_int;  
//     uint8_t temp_deci; 
//     uint8_t check_sum; 
// } BSP_DHT11Data_TypeDef;

// void BSP_DHT11_Init(void);
// uint8_t BSP_DHT11_ReadData(BSP_DHT11Data_TypeDef *DHT11_Data);

#define DHT11_Pin GPIO_PIN_9
#define DHT11_GPIO_Port GPIOB

#define DHT11_DQ_OUT_HIGH HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET)
#define DHT11_DQ_OUT_LOW HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET)
#define DHT11_DQ_IN HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)

void DHT11_IO_IN(void);
void DHT11_IO_OUT(void);
uint8_t DHT11_Init(void);
void DHT11_Rst(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint16_t *temp, uint16_t *humi);

extern uint16_t tem;
extern uint16_t him;

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_DHT11_H__ */
