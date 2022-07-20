/*
 ******************************************************************************
 * @file		:bsp_dac.h
 * @brief		:The board support package for dac.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.27
 ******************************************************************************
 */
 
#ifndef __BSP_DAC_H__
#define __BSP_DAC_H__
 
#include "bsp_config.h"
 
/* ============================== */
#define SinWaveout 						0
#define LisaruImage						1
/* ============================== */
/* -@- */


 
#if		SinWaveout
extern const uint16_t  Sine64bit[]; 
extern const uint16_t	 Sine128bit[];
extern const uint16_t	 Sine128bit_0[];
extern const uint16_t	 Sine128bit_45[];
extern const uint16_t	 Sine128bit_90[];
extern const uint16_t	 Sine128bit_135[];
extern const uint16_t	 Sine128bit_180[];
extern const uint16_t  Sine256bit[];
extern const uint16_t  Sine256bit_0[];
extern const uint16_t  Sine256bit_45[];
extern const uint16_t  Sine256bit_90[];
extern const uint16_t  Sine256bit_135[];
extern const uint16_t  Sine512bit_180[];
extern const uint16_t  Sine1024bit[]; 
extern const uint16_t  Sine2048bit[]; 
#endif /* SinWaveout */

#if		LisaruImage

#define Lisaru_NumSize		800

typedef struct lisaru_img_num {
	const uint16_t x[Lisaru_NumSize];
	const uint16_t y[Lisaru_NumSize];
	uint32_t len;
}lisaruimgTypedef;

extern const lisaruimgTypedef* lisaru_Num[];
extern const lisaruimgTypedef* lisaru_Alp[];



extern uint16_t lisaru_counts;
#define __Lisaru_Draw_Num(n)					{	HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, lisaru_Num[n]->x[lisaru_counts]); \
																				HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_2, DAC_ALIGN_12B_R, lisaru_Num[n]->y[lisaru_counts]); \
																				lisaru_counts++; \
																				if(lisaru_counts >= lisaru_Num[n]->len)lisaru_counts = 0;}
#define __Lisaru_Draw_Alp(n)					{	HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, lisaru_Alp[n]->x[lisaru_counts]); \
																				HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_2, DAC_ALIGN_12B_R, lisaru_Alp[n]->y[lisaru_counts]); \
																				lisaru_counts++; \
																				if(lisaru_counts >= lisaru_Alp[n]->len)lisaru_counts = 0;}
#endif /* LisaruImage */

void bsp_DAC_TriggerNONE(void);
void bsp_DAC_TriggerTim(void);																				
																				
#endif

