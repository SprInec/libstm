/**
 * @file bsp_decode.h
 * @author July (Email: JulyCub@163.com)
 * @brief The board support package for decode.
 * @version 0.1
 * @date 2022.06.28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_DECODE_H__
#define __BSP_DECODE_H__

#include "bsp_config.h"

/* 上升沿检测 */
#define __BSP_TOPEDGE_DET(__DATA1__, __DATA2__) ((((__DATA2__) - (__DATA1__)) == 1) ? 1 : 0)
/* 下降沿检测 */
#define __BSP_FALEDGE_DET(__DATA1__, __DATA2__) ((((__DATA1__) - (__DATA2__)) == 1) ? 1 : 0)

float BSP_Arry16ToUFloat(uint16_t arry[]);
float BSP_Arry24ToUFloat(uint8_t arry[]);
void BSP_DataNormalization(uint16_t data[], uint16_t result[], uint16_t len, uint16_t threshold);
void BSP_DigitalDetector(uint16_t *data, uint16_t *result, uint16_t data_len, uint16_t bitlen);
void BSP_DataStandardization(uint16_t *data, uint16_t datalen, uint16_t bitlen, uint8_t framenum);
void BSP_ReadcodeFromArry(uint16_t *data, uint8_t *result, uint16_t datalen, uint16_t bitlen, uint8_t framenum);
uint8_t BSP_ExtractEffectiveData(uint16_t *data, uint16_t *result, uint16_t data_len,
                                uint16_t bitlen, uint16_t headtaillen);
uint8_t BSP_ExtractEffectiveData_v2(uint16_t *data, uint16_t *result, uint16_t data_len,
                                    uint16_t bitlen, uint16_t headtaillen);
uint8_t BSP_ExtractEffectiveData_v3(uint16_t *data, uint16_t *result, uint16_t data_len, uint16_t frame_len);
void BSP_CumMonitorEdgePairs(uint16_t *data, uint16_t *result, uint16_t datalen, uint16_t threshold);
void BSP_SumValueStandard(uint16_t *data, uint16_t *result, uint16_t data_len, uint8_t bits, uint16_t threshold);

#endif
