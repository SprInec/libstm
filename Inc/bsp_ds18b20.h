/**
 ******************************************************************************
 * @file		:bsp_ds18b20.h
 * @brief		:The board support package for DS18B20.
 * @version		:1.0
 * @from		:GitHub
 * @author		:FiYu
 * @date		:2015-4-12
 * @update		:2023.02.27	-v1
 * @state		:Enable
 ******************************************************************************
 */

#ifndef __BSP_DS18B20_H__
#define __BSP_DS18B20_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"
#include "gpio.h"

#define HIGH 1
#define LOW 0

#define DS18B20_PIN GPIO_PIN_13
#define DS18B20_PORT GPIOB

#define DS18B20_DATA_OUT(a)                                         \
	if (a)                                                          \
		HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET); \
	else                                                            \
		HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET)

#define DS18B20_DATA_IN() HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)
typedef struct
{
	uint8_t humi_int;  // 湿度的整数部分
	uint8_t humi_deci; // 湿度的小数部分
	uint8_t temp_int;  // 温度的整数部分
	uint8_t temp_deci; // 温度的小数部分
	uint8_t check_sum; // 校验和

} DS18B20_Data_TypeDef;

uint8_t DS18B20_Init(void);
float DS18B20_Get_Temp(void);

#ifdef __cplusplus
}
#endif
#endif /* __DS18B20_H */
