/**
 * @file bsp_bmp280.hpp
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
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

#define BMP280_GPIO_PORT GPIOB
#define BMP280_SDA_PIN GPIO_PIN_5
#define BMP280_SCL_PIN GPIO_PIN_7

// BMP工作模式 
typedef enum {
    BMP280_SLEEP_MODE = 0x0,
    BMP280_FORCED_MODE = 0x1, // 可以说0x2
    BMP280_NORMAL_MODE = 0x3
} BMP280_WORK_MODE;

// BMP压力过采样因子
typedef enum
{
    BMP280_P_MODE_SKIP = 0x0, /*skipped*/
    BMP280_P_MODE_1,          /*x1*/
    BMP280_P_MODE_2,          /*x2*/
    BMP280_P_MODE_3,          /*x4*/
    BMP280_P_MODE_4,          /*x8*/
    BMP280_P_MODE_5           /*x16*/
} BMP280_P_OVERSAMPLING;

// BMP温度过采样因子
typedef enum
{
    BMP280_T_MODE_SKIP = 0x0, /*skipped*/
    BMP280_T_MODE_1,          /*x1*/
    BMP280_T_MODE_2,          /*x2*/
    BMP280_T_MODE_3,          /*x4*/
    BMP280_T_MODE_4,          /*x8*/
    BMP280_T_MODE_5           /*x16*/
} BMP280_T_OVERSAMPLING;

// IIR滤波器时间常数
typedef enum
{
    BMP280_FILTER_OFF = 0x0, /*filter off*/
    BMP280_FILTER_MODE_1,
    /*0.223*ODR*/ /*x2*/
    BMP280_FILTER_MODE_2,
    /*0.092*ODR*/ /*x4*/
    BMP280_FILTER_MODE_3,
    /*0.042*ODR*/                      /*x8*/
    BMP280_FILTER_MODE_4 /*0.021*ODR*/ /*x16*/
} BMP280_FILTER_COEFFICIENT;

// 保持时间
typedef enum
{
    BMP280_T_SB1 = 0x0, /*0.5ms*/
    BMP280_T_SB2,       /*62.5ms*/
    BMP280_T_SB3,       /*125ms*/
    BMP280_T_SB4,       /*250ms*/
    BMP280_T_SB5,       /*500ms*/
    BMP280_T_SB6,       /*1000ms*/
    BMP280_T_SB7,       /*2000ms*/
    BMP280_T_SB8,       /*4000ms*/
} BMP280_T_SB;

typedef struct
{
    /* T1~P9 为补偿系数 */
    uint16_t T1;
    int16_t T2;
    int16_t T3;
    uint16_t P1;
    int16_t P2;
    int16_t P3;
    int16_t P4;
    int16_t P5;
    int16_t P6;
    int16_t P7;
    int16_t P8;
    int16_t P9;
} BMP280;

typedef struct
{
    BMP280_P_OVERSAMPLING P_Osample;
    BMP280_T_OVERSAMPLING T_Osample;
    BMP280_WORK_MODE WORKMODE;
} BMP_OVERSAMPLE_MODE;

typedef struct
{
    BMP280_T_SB T_SB;
    BMP280_FILTER_COEFFICIENT FILTER_COEFFICIENT;
    FunctionalState SPI_EN;
} BMP_CONFIG;

extern BMP280 *bmp280;

typedef long signed int BMP280_S32_t; 
typedef long unsigned int BMP280_U32_t; 
typedef long long signed int BMP280_S64_t;

#define dig_T1 bmp280->T1
#define dig_T2 bmp280->T2
#define dig_T3 bmp280->T3

#define dig_P1 bmp280->P1
#define dig_P2 bmp280->P2
#define dig_P3 bmp280->P3
#define dig_P4 bmp280->P4
#define dig_P5 bmp280->P5
#define dig_P6 bmp280->P6
#define dig_P7 bmp280->P7
#define dig_P8 bmp280->P8
#define dig_P9 bmp280->P9

#define BMP280_ADDRESS 0x76    
#define BMP280_RESET_VALUE 0xB6 

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

#define BMP280_MEASURING 0x01
#define BMP280_IM_UPDATE 0x08

/*calibration parameters */
#define BMP280_DIG_T1_LSB_REG 0x88
#define BMP280_DIG_T1_MSB_REG 0x89
#define BMP280_DIG_T2_LSB_REG 0x8A
#define BMP280_DIG_T2_MSB_REG 0x8B
#define BMP280_DIG_T3_LSB_REG 0x8C
#define BMP280_DIG_T3_MSB_REG 0x8D
#define BMP280_DIG_P1_LSB_REG 0x8E
#define BMP280_DIG_P1_MSB_REG 0x8F
#define BMP280_DIG_P2_LSB_REG 0x90
#define BMP280_DIG_P2_MSB_REG 0x91
#define BMP280_DIG_P3_LSB_REG 0x92
#define BMP280_DIG_P3_MSB_REG 0x93
#define BMP280_DIG_P4_LSB_REG 0x94
#define BMP280_DIG_P4_MSB_REG 0x95
#define BMP280_DIG_P5_LSB_REG 0x96
#define BMP280_DIG_P5_MSB_REG 0x97
#define BMP280_DIG_P6_LSB_REG 0x98
#define BMP280_DIG_P6_MSB_REG 0x99
#define BMP280_DIG_P7_LSB_REG 0x9A
#define BMP280_DIG_P7_MSB_REG 0x9B
#define BMP280_DIG_P8_LSB_REG 0x9C
#define BMP280_DIG_P8_MSB_REG 0x9D
#define BMP280_DIG_P9_LSB_REG 0x9E
#define BMP280_DIG_P9_MSB_REG 0x9F

void BSP_BMP280_Init(void);
uint8_t BSP_BMP280_ReadID(void);
double BSP_BMP280_Get_Pressure(void);
double BSP_BMP280_Get_Temperature(void);

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_BMP280_H__*/
