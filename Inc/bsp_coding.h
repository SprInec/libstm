/*
 ******************************************************************************
 * @file		:bsp_coding.h
 * @brief		:The board support package for digital coding.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.27	-v0
 * @undate      :2023.02.27 -v1
 ******************************************************************************
 */

#ifndef __BSP_CODING_H__
#define __BSP_CODING_H__

#include "bsp_config.h"

/* -@- */
#if 0
#define __RZ
#elif 0
#define __NRZ
#elif 0
#define __MCST
#elif 0
#define __DFMCST
#endif

/* 巴克码序列 */
extern int bsp_baker7bit[];
extern int bsp_baker11bit[];
extern int bsp_baker13bit[];
/* 巴克码自相关系数 */
#define XCORR_BAKER_7 7
#define XCORR_BAKER_11 11
#define XCORR_BAKER_13 13

/* arr_char转二进制 */
void bsp_charToBin(char *num, uint16_t *code, uint8_t len);
/* 十进制转二进制 */
void bsp_DecToBin(int num, uint16_t code[], unsigned char len);
/* 浮点数转二进制 */
void bsp_FloatToBin(float num, uint16_t code[]);
void bsp_FloatToBCD(float num, uint16_t code[]);
/* 归零编码 */
_Bool RZ(uint16_t str[], uint16_t code[], char len);
/* 不归零编码 */
_Bool NRZ(uint16_t str[], uint16_t code[], char len);
/* 曼彻斯特编码 */
_Bool MCST(uint16_t str[], uint16_t code[], char len);
/* 曼彻斯特解码 */
_Bool MCST_Decode(uint16_t code[], uint16_t decode[]);
/* 差分曼彻斯特编码 */
_Bool DFMCST(uint16_t str[], uint16_t code[], char len);

#endif
