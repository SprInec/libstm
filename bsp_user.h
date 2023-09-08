/**
 * @file bsp_user.h
 * @author July (Email: JulyCub@163.com)
 * @brief
 * @version 0.1
 * @date 2023.08.19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_USER_H__
#define __BSP_USER_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

/* data interface usart/serial port */
#define DATA_INTERFACE_UART_PORT huart5

#define THREAD_PRIORITY 10
#define THREAD_STACK_SIZE 512
#define THREAD_TIMESLICE 20

/* the sensor gather gap time */
#define SENSOR_DELAY_TIME 1000 * 10

/* weather data stuct */
typedef struct WET_DATA
{
    uint8_t UVR;    /* 紫外线辐射 */
    float TS;       /* 温度 */
    float HS;       /* 湿度 */
    double PM2_5;   /* 空气质量 */
    double BPS;     /* 气压 */
} BSP_WeatherData_TypeDef;

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_USER_H__ */
