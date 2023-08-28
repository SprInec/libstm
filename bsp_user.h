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
typedef struct WET_DATA
{
    uint8_t uv;
    float temperature;
    uint32_t ct[2];
} BSP_WeatherData_TypeDef;

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_USER_H__ */
