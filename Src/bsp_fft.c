/**
 ******************************************************************************
 * @file			:	bsp_fft.c
 * @brief			:	Board Support Packge for FFT and IFFT ...
 * @version		:	0.1.0
 * @author		: July
 * @data			:	2022.07.03
 ******************************************************************************
 * @part			:	one
 * @source		:	CSDN
 * @brief			:	Implementation of Fast Fourier Transform(FFT)
 *					 		and reversal Fast Fourier Transform(IFFT)
 * @version		:	--
 * @author		:	monkeyzx
 * @date			:	2013.08.05
 ******************************************************************************
 * @part			:	two
 * @brief			:	the fft based on dsp lib
 * @version		:	1.0
 * @data			:	2022.07.03
 ******************************************************************************
 */
 
#include <math.h>
#include <stdlib.h>
#include "arm_math.h"
#include "bsp_fft.h"


float32_t fft_handle_data[FFT_DATA_LENGTH*2];
float32_t	fft_output_data[FFT_DATA_LENGTH];
float32_t fft_maxdata;
float32_t fft_freq;
uint32_t 	fft_place;
 
float bsp_arm_fft_getfreq(void)
{
	for(uint16_t i = 0; i < FFT_DATA_LENGTH; i++)
	{
		fft_handle_data[2*i] = (data_buff[i]*1.0)*3.3/4096.0;
		fft_handle_data[2*i+1] = 0;
	}
	/* fft */
	arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_handle_data, 0, 1);
	/* 频谱输出 */
	arm_cmplx_mag_f32(fft_handle_data, fft_output_data, FFT_DATA_LENGTH);
	fft_output_data[0] /= FFT_DATA_LENGTH;
	for(int i = 1; i < FFT_DATA_LENGTH / 2; i++)
	{
		fft_output_data[i] /= FFT_DATA_LENGTH/2;
	}
	for(uint16_t i = 0; i < 10; i++)
	{
		fft_output_data[i] = 0;
	}
	/* 频率 = 下标*采样率/点数 */
	arm_max_f32(fft_output_data, FFT_DATA_LENGTH/2, &fft_maxdata, &fft_place);
	fft_freq = ((fft_place)*FFT_SMPELE_FREQ/FFT_DATA_LENGTH);
	
#if	0
	PRINTF_DEBUG("%d\n", wave_freq);
#endif
	
#if	0
		for(uint16_t i = 0; i < FFT_DATA_LENGTH/2 ;i++){
			PRINTF_DEBUG("%f\n", fft_output_data[i]);
		}
#endif
		
		return fft_freq;
	
}
 
 
/*
 * Bit Reverse
 * === Input ===
 * x : complex numbers
 * n : nodes of FFT. @N should be power of 2, that is 2^(*)
 * l : count by bit of binary format, @l=CEIL{log2(n)}
 * === Output ===
 * r : results after reversed.
 * Note: I use a local variable @temp that result @r can be set
 * to @x and won't overlap.
 */
static void BitReverse(complex *x, complex *r, int n, int l)
{
	int i = 0;
	int j = 0;
	short stk = 0;
	static complex *temp = 0;
 
	temp = (complex *)malloc(sizeof(complex) * n);
	if (!temp) {
		return;
	}
 
	for(i=0; i<n; i++) {
		stk = 0;
		j = 0;
		do {
			stk |= (i>>(j++)) & 0x01;
			if(j<l)
			{
				stk <<= 1;
			}
		}while(j<l);
 
		if(stk < n) {             /* 满足倒位序输出 */
			temp[stk] = x[i];
		}
	}
	/* copy @temp to @r */
	for (i=0; i<n; i++) {
		r[i] = temp[i];
	}
	free(temp);
}
 
/*
 * FFT Algorithm
 * === Inputs ===
 * x : complex numbers
 * N : nodes of FFT. @N should be power of 2, that is 2^(*)
 * === Output ===
 * the @x contains the result of FFT algorithm, so the original data
 * in @x is destroyed, please store them before using FFT.
 */
int zx_fft(complex *x, int N)
{
	int i,j,l,ip;
	static int M = 0;
	static int le,le2;
	static FFT_TYPE sR,sI,tR,tI,uR,uI;
 
	M = (int)(log(N) / log(2));
 
	/*
	 * bit reversal sorting
	 */
	BitReverse(x,x,N,M);
 
	/*
	 * For Loops
	 */
	for (l=1; l<=M; l++) {   /* loop for ceil{log2(N)} */
		le = (int)pow(2,l);
		le2 = (int)(le / 2);
		uR = 1;
		uI = 0;
		sR = cos(PI / le2);
		sI = -sin(PI / le2);
		for (j=1; j<=le2; j++) {   /* loop for each sub DFT */
			//jm1 = j - 1;
			for (i=j-1; i<=N-1; i+=le) {  /* loop for each butterfly */
				ip = i + le2;
				tR = x[ip].real * uR - x[ip].img * uI;
				tI = x[ip].real * uI + x[ip].img * uR;
				x[ip].real = x[i].real - tR;
				x[ip].img = x[i].img - tI;
				x[i].real += tR;
				x[i].img += tI;
			}  /* Next i */
			tR = uR;
			uR = tR * sR - uI * sI;
			uI = tR * sI + uI *sR;
		} /* Next j */
	} /* Next l */
 
	return 0;
}
 
/*
 * Inverse FFT Algorithm
 * === Inputs ===
 * x : complex numbers
 * N : nodes of FFT. @N should be power of 2, that is 2^(*)
 * === Output ===
 * the @x contains the result of FFT algorithm, so the original data
 * in @x is destroyed, please store them before using FFT.
 */
int zx_ifft(complex *x, int N)
{
	int k = 0;
 
	for (k=0; k<=N-1; k++) {
		x[k].img = -x[k].img;
	}
 
	zx_fft(x, N);    /* using FFT */
 
	for (k=0; k<=N-1; k++) {
		x[k].real = x[k].real / N;
		x[k].img = -x[k].img / N;
	}
 
	return 0;
}
 
/*
 * Code below is an example of using FFT and IFFT.
 */

complex bsp_fft_data[FFT_SAMPLE_NODES];
int INPUT[FFT_SAMPLE_NODES];
int OUTPUT[FFT_SAMPLE_NODES];
 
static void zx_MakeInput()
{
	int i;
 
	for ( i=0;i<FFT_SAMPLE_NODES;i++ )
	{
		bsp_fft_data[i].real = sin(PI*2*i/FFT_SAMPLE_NODES);
		bsp_fft_data[i].img = 0.0f;
		INPUT[i]=sin(PI*2*i/FFT_SAMPLE_NODES)*1024;
	}
}
 
static void zx_MakeOutput()
{
	int i;
 
	for ( i=0;i<FFT_SAMPLE_NODES;i++ )
	{
		OUTPUT[i] = sqrt(bsp_fft_data[i].real*bsp_fft_data[i].real + bsp_fft_data[i].img*bsp_fft_data[i].img)*1024;
	}
}
 
void bsp_zx_fft(void)
{
	zx_MakeInput();
 
	zx_fft(bsp_fft_data,FFT_SAMPLE_NODES);
	zx_MakeOutput();
 
	zx_ifft(bsp_fft_data,FFT_SAMPLE_NODES);
	zx_MakeOutput();
}