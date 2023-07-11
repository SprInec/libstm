/**
 ******************************************************************************
 * @file		:bsp_ad9833.h
 * @brief		:AD9833 module driver.
 * @version	:0.1.1
 * @author	:merchant
 * @date		:--
 ******************************************************************************
 * @chip		:STM32
 * @pinset	:
 *							GPIOE_PIN_0 	---> FSYNC
 *							GPIOE_PIN_1 	---> SCK
 *							GPIOE_PIN_2 	---> DAT
 *							GPIOE_PIN_3   	---> CS
 *
 * @if MULT
 *							GPIOB_PIN_15 	---> FSYNC1
 *							GPIOB_PIN_14 	---> SCK1
 *							GPIOB_PIN_13 	---> DAT1
 *							GPIOB_PIN_12	---> CS1
 *
 *						 	GPIOC_PIN_0		---> FSYNC2
 *							GPIOC_PIN_1		---> SCK2
 *							GPIOC_PIN_2		---> DAT2
 *							GPIOC_PIN_3		---> CS2
 *
 *******************************************************************************
 */

#ifndef __BSP_AD9833_H__
#define __BSP_AD9833_H__

#include "bsp_config.h"

/* ============== 双AD9833 ============= */
#define AD9833_DEMULT 1 /* 1->单 0->双 */
/* ===================================== */

/* -@- */
#if 1
#undef SofeSpi
#define SofeSpi
#else
#undef HardSpi
#define HardSpi
#endif

#define TRI_WAVE 0 // 输出三角波
#define SIN_WAVE 1 // 输出正弦波
#define SQU_WAVE 2 // 输出方波

#ifdef SofeSpi

#define _1 GPIO_PIN_SET
#define _0 GPIO_PIN_RESET

#define CS_0() CS(_0)
#define CS_1() CS(_1)

#endif

#if AD9833_DEMULT
/*端口定义 */
#define FSY(n) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, n)
#define SCK(n) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, n)
#define DAT(n) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, n)
#define CS(n) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, n)

void AD9833_Init(void);
void AD9833_WaveSeting(double frequence, unsigned int frequence_SFR, unsigned int WaveMode, unsigned int Phase);
void AD9833_AmpSet(unsigned char amp);
void MCP41010_SetRes(uint8_t xRes);
void MCP41010_SPI_SendOneByte(uint8_t xtemp);
void AD9833_Write(unsigned int TxData);
#if 0
void MCP41010_SetRes(uint8_t xRes);

void bsp_AD9833_IO_Init(void);
void bsp_AD9833_SPI_Init(void);
void bsp_mcp41_init(uint8_t pot_value);
void bsp_mcp41_set_value(uint8_t pot_id, uint8_t pot_value);
#endif
#else
/*端口定义 */
#define FSY1(n) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, n)
#define SCK1(n) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, n)
#define DAT1(n) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, n)
#define CS1(n) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, n)

#define FSY2(n) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, n)
#define SCK2(n) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, n)
#define DAT2(n) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, n)
#define CS2(n) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, n)

void AD9833_MULT_AmpSet1(unsigned char amp);
void AD9833_MULT_AmpSet2(unsigned char amp);
void AD9833_MULT_WaveSeting1(double Freq, unsigned int Freq_SFR, unsigned int WaveMode, unsigned int Phase);
void AD9833_MULT_WaveSeting2(double Freq, unsigned int Freq_SFR, unsigned int WaveMode, unsigned int Phase);
#endif

#endif /* __BSP_AD9833_H__ */
