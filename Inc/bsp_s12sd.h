/**
 * @file bsp_s12sd.h
 * @author July (Email: JulyCub@163.com)
 * @brief S12SD Hardware
 * @version 0.1
 * @date 2023.08.28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_S12SD_H__
#define __BSP_S12SD_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

void BSP_S12SD_Init(void);
uint16_t BSP_S12SD_Read(void);
uint8_t BSP_S12SD_UV_Index(uint16_t value);

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_S12SD_H__ */
