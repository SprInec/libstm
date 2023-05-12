/**
 ******************************************************************************
 * @file		:bsp_decode.h
 * @brief		:The board support package for decode.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.28
 ******************************************************************************
 */

#ifndef __BSP_DECODE_H__
#define __BSP_DECODE_H__

#include "bsp_config.h"

void print_binary(unsigned int number);
_Bool bsp_SyncDetection(unsigned int data[]);
_Bool bsp_SyncEnd(unsigned int data[]);
unsigned char bsp_SyncRx1Byte(unsigned int data[]);
void bsp_Get1ByetfromArray(unsigned int data[], unsigned int result[], char startlen, int len);
_Bool bsp_SyncRxData(unsigned int data[], unsigned char result[]);
float bsp_Arry16ToUFloat(uint16_t arry[]);
uint16_t bsp_Arry8ToUInt(uint16_t arry[]);

#endif
