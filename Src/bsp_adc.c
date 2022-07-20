/**
 ******************************************************************************
 * @file		:bsp_adc.c
 * @brief		:The board support package for ADC.
 * @version		:0.1.0
 * @author		:July
 * @date		:2022.06.30
 ******************************************************************************
 */
 
 #include "bsp_adc.h"

 
 /**
	*		模拟转等量数字数组 
	* 	data > 0	--->  1 
	*		data = 0	--->	0
	*/
_Bool AnlogToDigit(uint16_t data[], uint16_t result[], uint16_t len)
{
	
	for(uint16_t i = 0; i < len; i++)
	{
		if(data[i] > 0)
		{
			result[i] = 1;
		}
		else if(data[i] == 0 && data[i+1] == 0)
		{
			result[i] = 0;
		}
	}
	return 1;
}



