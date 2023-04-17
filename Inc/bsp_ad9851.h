/**
 ******************************************************************************
 * @file			: bsp_ad9851.h
 * @brief			: AD9851 Driver.
 * @version		    : 0.1.0
 * @author		    : 康威电子
 * @date			: 2022.07.27
 ******************************************************************************
 * @pinset
 *              PA4     --> CLK
 *              PA5     --> FQ
 *              PA6     --> RST   
 *              PC0     --> D0
 *              PC1     --> D1
 *              PC2     --> D2
 *              PC3     --> D3
 *              PC4     --> D4
 *              PC5     --> D5
 *              PC6     --> D6
 *              PC7     --> D7
 *
 ******************************************************************************
 */
#ifndef __AD9851_H
#define __AD9851_H	 

#include "bsp_config.h"
#include "gpio.h"

/* ============================== */
#define USE_AD9851_SER  /* 串行 */
//#define USE_AD9851_PAR  /* 并行 */
/* ============================== */

#ifdef USE_AD9851_SER

#define AD9851_w_clk    PAout(4)  
#define AD9851_fq_up    PAout(5)  
#define AD9851_rest     PAout(6)  
#define AD9851_bit_data PCout(7)  
#define AD9851_DataBus	GPIOC->BSRR
#define D0   		    PCout(0)
#define D1   		    PCout(1)
#define D2   		    PCout(2)


void AD9851_reset_serial(void);
void AD9851_wr_serial(uint8_t w0, double frequence);
void AD9851_IO_Init(void);
void AD9851_Setfq(double fq);
void AD9851_Init(void);

#endif
#ifdef USE_AD9851_PAR
#define AD9851_w_clk    PAout(4)  
#define AD9851_fq_up    PAout(3)  
#define AD9851_rest     PAout(6)  
#define AD9851_bit_data PCout(7)  
#define AD9851_DataBus	GPIOC->BSRR
#define D0   		    PCout(0)
#define D1   		    PCout(1)
#define D2   		    PCout(2)


void AD9851_reset(void);
void AD9851_wr_parrel(u8 w0,double frequence);
void AD9851_IO_Init(void);
void AD9851_Setfq(double fq);
void AD9851_Init(void);
#endif
#endif

