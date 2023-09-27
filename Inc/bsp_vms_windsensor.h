/**
 * @file bsp_vms_windsensor.h
 * @author July (Email: JulyCub@163.com)
 * @brief VMS WIND SPEED SENSOR HARDWARE
 * @version 0.1
 * @date 2023.09.27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_VMSWINDSENSOR_H__
#define __BSP_VMSWINDSENSOR_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"

extern uint8_t VMSCALLBACK_DATA[8];

void BSP_VMS_WindSensor_Init(void);
uint8_t BSP_VMS_WindSensor_InqureWindSpeed(void);

#ifdef __cplusplus
}
#endif
#endif /* !__BSP_VMSWINDSENSOR_H__ */
