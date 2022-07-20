#include "bsp_dwt.h"

DWTCLass Dwt_1;

uint32_t DWT_GetSystick(void)
{
	return SystemCoreClock;
}
uint8_t  DWT_Init(void)
{
	if(Dwt_1.dwtsystick == 0)
	{
		Dwt_1.dwtsystick = Dwt_1.Bsp_GetSystick();
		#if defined(DEBUG_USART)
		PRINTF_DEBUG("DWT SYStcik : %d\n", Dwt_1.dwtsystick);
		#endif
	}
	if(Dwt_1.OneCountTime == 0)
	{
		Dwt_1.OneCountTime = Dwt_1.dwtsystick / 1000 / 1000; //us
	}
	/*使能DWT模块*/
	#if defined(DEBUG_USART)
	PRINTF_DEBUG("DWT Cyccnt : %d\n", DWT->CYCCNT);
	#endif
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; 
	DWT->CTRL        |= DWT_CTRL_CYCCNTENA_Msk;
	#if defined(DEBUG_USART)
	PRINTF_DEBUG("DWT Cyccnt : %d\n", DWT->CYCCNT);
	#endif
	return 0;
}
/*内联时要外部引用一下*/
inline uint32_t DWT_GetCount(void)
{
	return (uint32_t)(DWT->CYCCNT);
}
uint32_t DWT_GetUnlityCount(DWTCLass *_dwt)
{
	if((_dwt)->OneCountTime == 0 || (_dwt)->dwtsystick == 0)
	{
		(_dwt)->Bsp_DWT_Init();
	}
	(_dwt)->TimeCount.UtilityTime = (_dwt)->TimeCount.StopTime - (_dwt)->TimeCount.BeginTime;
	(_dwt)->TimeCount.UtilityTime = (_dwt)->TimeCount.UtilityTime / (_dwt)->OneCountTime;
	return (uint32_t)(_dwt)->TimeCount.UtilityTime;
}
void DWT_DELAY_US( struct DWTCLass *_dwt,uint32_t us)
{
	(_dwt)->TimeCount.BeginTime = (_dwt)->Bsp_GetCount();
	if((_dwt)->OneCountTime == 0 || (_dwt)->dwtsystick == 0)
	{
		(_dwt)->Bsp_DWT_Init();
	}
	(_dwt)->TimeCount.Timeing  = (us * (_dwt)->OneCountTime);
	(_dwt)->TimeCount.StopTime = (_dwt)->TimeCount.BeginTime + (_dwt)->TimeCount.Timeing;
	if((_dwt)->TimeCount.StopTime > (_dwt)->TimeCount.BeginTime)
	{
		while((_dwt)->Bsp_GetCount() < (_dwt)->TimeCount.StopTime);
	}
	else
	{
		while((_dwt)->Bsp_GetCount() >= (_dwt)->TimeCount.StopTime);
		while((_dwt)->Bsp_GetCount() < (_dwt)->TimeCount.StopTime);
	}
}
DWTCLass Dwt_1 = {
	.OneCountTime							 = 0,
	.TimeCount      					 = {0},
	.dwtsystick     					 = 0,
	.Bsp_GetSystick 					 = DWT_GetSystick,
	.Bsp_DWT_Init   					 = DWT_Init,
	.Bsp_GetCount   					 = DWT_GetCount,
	.TimeCount.Bsp_DWTGetCount = DWT_GetUnlityCount,
	.Bsp_DWT_Delay_us          = (void *)DWT_DELAY_US,
};

















