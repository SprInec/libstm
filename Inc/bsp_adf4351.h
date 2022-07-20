#ifndef _ADF4351_H_
#define _ADF4351_H_

#include "main.h"
#include "gpio.h"

/* Êä³ö */
#define ADF4351_CE(n)							HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, n) 
#define ADF4351_LE(n) 						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, n)
#define ADF4351_OUTPUT_DATA(n)		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, n)
#define ADF4351_CLK(n)						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, n)

/* ¶ÁÈ¡ */
#define ADF4351_RF_OFF	((uint32_t)0XEC801C)

void ADF4351Init(void);
void ReadToADF4351(uint8_t count, uint8_t *buf);
void WriteToADF4351(uint8_t count, uint8_t *buf);
void WriteOneRegToADF4351(uint32_t Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre);		//	(xx.x) M Hz

#endif

