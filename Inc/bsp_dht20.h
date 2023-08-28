/**
 * @file bsp_dht20.h
 * @author July (Email: JulyCub@163.com)
 * @brief DHT20 Hardware
 * @version 0.1
 * @date 2023.08.28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_DHT20_H__
#define __BSP_DHT20_H__
#include "bsp_config.h"

#define DHT20_GPIO_PORT GPIOE
#define DHT20_SDA_PIN GPIO_PIN_3
#define DHT20_SCL_PIN GPIO_PIN_5

void BSP_DHT20_Init(void);
void BSP_DHT20_Read_CTdata(uint32_t *ct);             // 没有CRC校验，直接读取AHT20的温度和湿度数据
void BSP_DHT20_Read_CTdata_CRC(uint32_t *ct);         // CRC校验后，读取AHT20的温度和湿度数据
uint8_t Calc_CRC8(uint8_t *message, uint8_t Num); // CRC校验函数

#endif /* !__BSP_DH20_H__ */
