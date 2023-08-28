/**
 * @file bsp_bmp280.h
 * @author July (Email: JulyCub@163.com)
 * @brief BMP280 Hardware
 * @version 0.1
 * @date 2023.08.28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_BMP280_H__
#define __BSP_BMP280_H__
#include "bsp_config.h"

#define BMP280_ADDRESS 0x76     // 从设备地址
#define BMP280_RESET_VALUE 0xB6 // 复位寄存器写入值

#define BMP280_CHIPID_REG 0xD0           /*Chip ID Register */
#define BMP280_RESET_REG 0xE0            /*Softreset Register */
#define BMP280_STATUS_REG 0xF3           /*Status Register */
#define BMP280_CTRLMEAS_REG 0xF4         /*Ctrl Measure Register */
#define BMP280_CONFIG_REG 0xF5           /*Configuration Register */
#define BMP280_PRESSURE_MSB_REG 0xF7     /*Pressure MSB Register */
#define BMP280_PRESSURE_LSB_REG 0xF8     /*Pressure LSB Register */
#define BMP280_PRESSURE_XLSB_REG 0xF9    /*Pressure XLSB Register */
#define BMP280_TEMPERATURE_MSB_REG 0xFA  /*Temperature MSB Reg */
#define BMP280_TEMPERATURE_LSB_REG 0xFB  /*Temperature LSB Reg */
#define BMP280_TEMPERATURE_XLSB_REG 0xFC /*Temperature XLSB Reg */
// 状态寄存器转换标志
#define BMP280_MEASURING 0x01
#define BMP280_IM_UPDATE 0x08

#endif /* !__BSP_BMP280_H__*/
