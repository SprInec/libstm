/**
 ******************************************************************************
 * @file		:bsp_usart_hmi.c
 * @brief		:The board support package for USART HMI.
 * @version		:0.1.3
 * @author		:July
 * @date		:2022.07.06
 ******************************************************************************
 * @updata		:ver 0.1.3
 * @add			:func -> bsp_HMI_RxOder
 * @attention	 please use bsp_HMI_RxOder in the UART CallBack Function it 
 * 				 will back the odr or data from HMI.
 ******************************************************************************
 */

#include "bsp_uart_hmi.h"
#include "string.h"

/* 宏用变量 */
uint8_t 	hmi_chnl;	// 通道
uint8_t 	hmi_id;		// ID
uint8_t 	hmi_page;	// 页数
uint16_t 	hmi_point; 
uint16_t 	hmi_point_counts;

const uint8_t arry1[256] = {
		0, 1, 1, 2, 3, 4, 5, 7, 9, 14, 17, 19, 21, 24,
		27, 19, 31, 36, 39, 47, 56, 67, 87, 98, 99, 100,
};

/* HMI串口指令接收 */
uint8_t bsp_HMI_RxOder(uint8_t* odr)
{
	uint8_t id;

	/* 指令接收 USART Oders */
	if(*odr == 'U')
	{
		switch(*(odr + 1))
		{
			case 0:	id = 0;	break;
			case 1:	id = 1;	break;
			case 2:	id = 2;	break;
			case 3:	id = 3;	break;
			case 4:	id = 4;	break;

			case 5:	id = 5;	break;
			case 6:	id = 6;	break;
			case 7:	id = 7;	break;
			case 8:	id = 8;	break;
			case 9:	id = 9;	break;
#ifdef HMI_MULT_ODR
#if ( HMI_MULT_ODR == 20 )

			case 'a':	id = 10;	break;
			case 'b':	id = 11;	break;
			case 'c':	id = 12;	break;
			case 'e':	id = 13;	break;
			case 'f':	id = 14;	break;

			case 'g':	id = 15;	break;
			case 'h':	id = 16;	break;
			case 'i':	id = 17;	break;
			case 'j':	id = 18;	break;
			case 'k':	id = 19;	break;

#elif ( HMI_MULT_ODR == 30 )

			case 'l':	id = 20;	break;
			case 'm':	id = 21;	break;
			case 'n':	id = 22;	break;
			case 'o':	id = 23;	break;
			case 'p':	id = 24;	break;

			case 'q':	id = 25;	break;
			case 'r':	id = 26;	break;
			case 's':	id = 27;	break;
			case 't':	id = 28;	break;
			case 'u':	id = 29;	break;

#elif ( HMI_MULT_ODR == 40 )

			case 'v':	id = 30;	break;
			case 'w':	id = 31;	break;
			case 'x':	id = 32;	break;
			case 'y':	id = 33;	break;
			case 'z':	id = 34;	break;

			case 'A':	id = 35;	break;
			case 'B':	id = 36;	break;
			case 'C':	id = 37;	break;
			case 'D':	id = 38;	break;
			case 'E':	id = 39;	break;
			
#elif ( HMI_MULT_ODR == 50 )

			case 'F':	id = 40;	break;
			case 'G':	id = 41;	break;
			case 'H':	id = 42;	break;
			case 'I':	id = 43;	break;
			case 'J':	id = 44;	break;

			case 'K':	id = 45;	break;
			case 'L':	id = 46;	break;
			case 'M':	id = 47;	break;
			case 'N':	id = 48;	break;
			case 'O':	id = 49;	break;
#endif
#endif
		}
	}
	/* 返回数据接收 CallBack */
	else if(*odr == 'B')
	{
		/* USER CODE */
	}
	return id;
}

/* 单通道特性透传曲线 */
void bsp_HMI_sendCurveSingle(uint8_t id, uint8_t chnl, uint8_t points[], uint16_t len, uint8_t page)
{
	uint16_t	i = len - 1;

#if  0
	__switchpage(page);
#endif
	__sendpoints(id, chnl, len);
	while(i >= 0){
		bsprif1("\"%d\"", points[i--]);
		if(i < len - 1){
			bsprif1("\",\"");
		}
	}
}
