/**
 ******************************************************************************
 * @file		:bsp_usart_hmi.c
 * @brief		:The board support package for USART HMI.
 * @version		:0.1.2
 * @author		:July
 * @date		:2022.07.06
 ******************************************************************************
 */

#include "bsp_hmi.h"

/* 宏用变量 */
uint8_t 	hmi_chnl;
uint8_t 	hmi_id;
uint8_t 	hmi_page;
uint16_t 	hmi_point;
uint16_t 	hmi_point_counts;

const uint8_t arry1[256] = {
		0, 1, 1, 2, 3, 4, 5, 7, 9, 14, 17, 19, 21, 24,
		27, 19, 31, 36, 39, 47, 56, 67, 87, 98, 99, 100,
};

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
