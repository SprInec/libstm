#ifndef __BSP_DWT_H_
#define __BSP_DWT_H_

#include "gpio.h"
	
#define  DEBUG_USART

#if defined(DEBUG_USART)
	#include "bsp_usart.h"
#endif

#if 	defined(STM32F4)
	#include "stm32f4xx.h"
#elif defined(STM32F1)
	#include "stm32f1xx.h"
#endif

#define  DWT_ENABLE_FLAG     ( 0x01 << 24) // DEMCR的DWT使能位  
#define  DWTCLass(_name_)         _name_

/*DWT计数单元*/
typedef struct {
	uint32_t BeginTime;
	uint32_t StopTime;
	uint32_t UtilityTime;
	uint32_t Timeing;
	uint32_t (*Bsp_DWTGetCount)();
}DWT_Time_Count;


typedef struct DWTCLass{
	uint32_t OneCountTime;
	DWT_Time_Count TimeCount;
	uint32_t dwtsystick;
/*函数*/	
	uint32_t (*Bsp_GetSystick)();
	uint8_t  (*Bsp_DWT_Init)();
	uint32_t (*Bsp_GetCount)();
	void     (*Bsp_DWT_Delay_us)(struct DWTCLass *_dwt,uint32_t us);
}DWTCLass;


uint32_t DWT_GetCount(void);



extern DWTCLass Dwt_1;



#endif
