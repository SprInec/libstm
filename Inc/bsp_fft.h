/**
 ******************************************************************************
 * @file		:bsp_fft.h
 * @brief		:The board support package for FFT.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.07.03
 ******************************************************************************
 */
 
#ifndef __BSP_FFT_H__
#define __BSP_FFT_H__

#include "bsp_config.h"



typedef float          FFT_TYPE;
 
#ifndef PI
#define PI             (3.14159265f)
#endif

#define  FFT_SAMPLE_NODES              (128u)
#define  FFT_SMPELE_FREQ								50000.0f
#define  FFT_DATA_LENGTH								1024u
 
typedef struct complex_st {
	FFT_TYPE real;
	FFT_TYPE img;
}complex;

extern float32_t 	fft_handle_data[DATA_LENGTH*2];
extern float32_t	fft_output_data[DATA_LENGTH];

int fft(complex *x, int N);
int ifft(complex *x, int N);

void bsp_zx_fft(void);
float bsp_arm_fft_getfreq(void);
 












#endif
