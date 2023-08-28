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

#define DHT11_HIGH GPIO_PIN_SET
#define DHT11_LOW GPIO_PIN_RESET

#define Bit_RESET 0
#define Bit_SET 1

#define DHT11_PORT GPIOE
#define DHT11_PIN GPIO_PIN_1
#define DHT11_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()

#define DHT11_DATA_IN() HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)
#define DHT11_DATA_OUT(value) HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, value)

typedef struct DHT11_DATA
{
    uint8_t humi_int;  // ʪ�ȵ���������
    uint8_t humi_deci; // ʪ�ȵ�С������
    uint8_t temp_int;  // �¶ȵ���������
    uint8_t temp_deci; // �¶ȵ�С������
    uint8_t check_sum; // У���
} BSP_DHT11Data_TypeDef;

void BSP_DHT11_Init(void);
uint8_t BSP_DHT11_ReadData(BSP_DHT11Data_TypeDef *DHT11_Data);

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_DHT11_H__ */
