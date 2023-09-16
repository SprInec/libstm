/**
 * @file bsp_s12sd.c
 * @author July (Email: JulyCub@163.com)
 * @brief S12SD Hardware
 * @version 0.1
 * @date 2023.08.28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_s12sd.h"
#include "adc.h"

/**
 * @brief S12SD Initialization
 */
void BSP_S12SD_Init(void)
{
    MX_ADC1_Init();
}

/**
 * @brief read S12SD's value
 * @return uint16_t mv
 */
uint16_t BSP_S12SD_Read(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    return (uint16_t)((HAL_ADC_GetValue(&hadc1) * 3300) / 4096);
}

/**
 * @brief The UV index transfor
 * @param value voltage value
 * @return uint8_t UV index 0-11
 */
uint8_t BSP_S12SD_UV_Index(uint16_t value)
{
    if (value < 50)
    {
        return 0;
    }
    else if (value < 227)
    {
        return 1;
    }
    else if (value < 318)
    {
        return 2;
    }
    else if (value < 408)
    {
        return 3;
    }
    else if (value < 503)
    {
        return 4;
    }
    else if (value < 606)
    {
        return 5;
    }
    else if (value < 696)
    {
        return 6;
    }
    else if (value < 795)
    {
        return 7;
    }
    else if (value < 881)
    {
        return 8;
    }
    else if (value < 976)
    {
        return 9;
    }
    else if (value < 1079)
    {
        return 10;
    }
    else
    {
        return 11;
    }
}
