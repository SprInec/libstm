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

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_DHT11_H__ */
