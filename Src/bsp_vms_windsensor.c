/**
 * @file bsp_vms_windsensor.c
 * @author July (Email: JulyCub@163.com)
 * @brief VMS WIND SPEED SENSOR HARDWARE
 * @version 0.1
 * @date 2023.09.27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_vms_windsensor.h"
#include "string.h"

/* 获取风速 */
const uint8_t VMSGET_WINDSPEED[] = {
    0x01, 0x03, 0x00, 0x00,
    0x00, 0x01, 0x84, 0x0A};
uint8_t VMSCALLBACK_DATA[8] = {0};

/**
 * @brief VMS风速传感器初始化
 */
void BSP_VMS_WindSensor_Init(void)
{
    BSP_RS485_Init();
}

/**
 * @brief 风速检测
 * @return uint8_t 真实值的10倍 m/s
 */
uint8_t BSP_VMS_WindSensor_InqureWindSpeed(void)
{
    if (RS485_SendData(VMSGET_WINDSPEED, 8) == RS485_SUCCESS)
    {
        RS485_ReadData(VMSCALLBACK_DATA, 8);
        return VMSCALLBACK_DATA[4];
    }
    else
    {
        return 0xFF;
    }
}
