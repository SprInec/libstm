/**
 ******************************************************************************
 * @file		:bsp_adc.h
 * @brief		:The board support package for ADC.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.30
 ******************************************************************************
 */

#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "bsp_config.h"

_Bool AnlogToDigit(uint16_t data[], uint16_t result[], uint16_t len);

#endif
