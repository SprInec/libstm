/**
 ******************************************************************************
 * @file		:bsp_usart_hmi.h
 * @brief		:The board support package for USART HMI.
 * @version	:0.1.2
 * @author	:July
 * @date		:2022.06.26
 ******************************************************************************
 */

#ifndef __BSP_USART_HMI_H__
#define __BSP_USART_HMI_H__

#include "bsp_config.h"
#include "bsp_usart.h"

/* 文本控件 */
typedef struct hmi_text{
	uint8_t			page;
	uint8_t			id;
	uint8_t			name;
	_Bool				drag;
	uint8_t			aph;
	uint8_t			effect;
	uint8_t			pco;
}hmitextTypedef;

/* 曲线/波形控件 */
typedef struct hmi_curve{
	uint8_t 		page;		
	uint8_t 		id;
	uint8_t 		chnl[4];
	uint8_t			name;
	_Bool				drag;
	uint8_t			aph;
	uint8_t			effect;
}hmicurveTypedef;

/* 曲线/波形控件通道宏定义 */
#define HMI_CHANNEL_1				1U
#define HMI_CHANNEL_2				2U
#define HMI_CHANNEL_3				3U
#define HMI_CHANNEL_4				4U

/* 宏用变量 */
extern uint8_t hmi_chnl;
extern uint8_t hmi_id;
extern uint8_t hmi_page;
extern uint16_t hmi_point;
extern uint16_t hmi_point_counts;


/* 指令结束帧 */
#define __prifend														bsprif1("\xff\xff\xff")
/* 切换页面 */
#define __switchpage(n)										{	bsprif1("page %d", hmi_page = n); \
																						__prifend;	}
/* 曲线控件增加一个点 */
#define __sendpoint(_ID,_CHNL,_POINT)			{	bsprif1("add %d,%d,%d", hmi_id = _ID, hmi_chnl = _CHNL, hmi_point = _POINT); \
																						__prifend;	}
/* 曲线控件添加多个点(透传) */
#define __sendpoints(_ID,_CHNL,_COUNT)		{ bsprif1("addt %d,%d,%d", hmi_id = _ID, hmi_chnl = _CHNL, hmi_point_counts = _COUNT); \
																						__prifend;	}

																		
																		
/* 单通道特性透传曲线 */																	
void bsp_HMI_sendCurveSingle(uint8_t id, uint8_t chnl, uint8_t points[], uint16_t len, uint8_t page);

#endif /* __BSP_USART_HMI_H__ */
