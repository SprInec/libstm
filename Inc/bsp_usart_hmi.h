/**
 * @file bsp_usart_hmi.h
 * @author July (Email: JulyCub@163.com)
 * @brief The board support package for USART HMI.
 * @version 1.0
 * @date 2022.07.06
 * @update 2023.07.31
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __BSP_USART_HMI_H__
#define __BSP_USART_HMI_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_config.h"
#include "bsp_usart.h"

/* 串口指令数 */
#define HMI_MULT_ODR 20

/* 文本控件 */
typedef struct
{
	const uint8_t page;		  /* 所属页面 */
	const uint8_t id;		  /* 控件 ID */
	const uint8_t objname[4]; /* 控件名称 */
	uint8_t drag;			  /* 是否支持拖动 0-否 1-是 */
	uint8_t aph;			  /* 不透明度 */
	uint8_t effect;			  /* 加载特效 */
	uint8_t pco;			  /* 字体色 */
	uint8_t font;			  /* 字库 */
	uint8_t pw;				  /* 是否显示为密码 */
} hmitextTypedef;

/* 曲线/波形控件 */
typedef struct
{
	const uint8_t page;		  /* 所属页面 */
	const uint8_t id;		  /* 控件 ID */
	const uint8_t chnl[4];	  /* 通道 */
	const uint8_t objname[4]; /* 控件名称 */
	uint8_t drag;			  /* 是否支持拖动 0-否 1-是 */
	uint8_t aph;			  /* 不透明度 */
	uint8_t effect;			  /* 加载特效 */
	uint8_t bco;			  /* 背景色 */
	uint8_t gdc;			  /* 网格颜色 */
	uint8_t gdw;			  /* 网格宽度 */
	uint8_t gdh;			  /* 网格高度 */
	uint16_t dis;			  /* 数据缩放百分比，最小10，最大1000 */
} hmicurveTypedef;

/* 按钮控件 */
typedef struct
{
	const uint8_t page;		  /* 所属页面 */
	const uint8_t id;		  /* 控件 ID */
	const uint8_t objname[4]; /* 控件名称 */
	uint8_t drag;			  /* 是否支持拖动 0-否 1-是 */
	uint8_t aph;			  /* 不透明度 */
	uint8_t effect;			  /* 加载特效 */
	uint8_t font;			  /* 字库 */
} hmibuttonTypedef;

/* 数字控件 */
typedef struct
{
	const uint8_t page;		  /* 所属页面 */
	const uint8_t id;		  /* 控件 ID */
	const uint8_t objname[4]; /* 控件名称 */
	uint8_t drag;			  /* 是否支持拖动 0-否 1-是 */
	uint8_t aph;			  /* 不透明度 */
	uint8_t effect;			  /* 加载特效 */
	uint8_t font;			  /* 字库 */
} hminumberTypedef;

/* 页面 */
typedef struct
{
	uint8_t id;
	uint8_t name[12];
	hmitextTypedef text[8];
	hmicurveTypedef curve;
	hmibuttonTypedef button[4];
} hmipageTypedef;

/* 曲线/波形控件通道宏定义 */
#define HMI_CHANNEL_1 1U
#define HMI_CHANNEL_2 2U
#define HMI_CHANNEL_3 3U
#define HMI_CHANNEL_4 4U

/* 全局变量 */
extern uint8_t hmi_chnl;
extern uint8_t hmi_id;
extern uint8_t hmi_page;
extern uint16_t hmi_point;
extern uint16_t hmi_point_counts;

/* 指令结束帧 */
#define __prifend(_huart_) bsprif(&_huart_, "\xff\xff\xff");
#define __prifend1 bsprif1("\xff\xff\xff")
#define __prifend2 bsprif2("\xff\xff\xff")
#define __prifend3 bsprif3("\xff\xff\xff")
/* 切换页面 */
#define __switchpage(__PAGE__)                     \
	{                                              \
		bsprif1("page %d", hmi_page = (__PAGE__)); \
		__prifend1;                                \
	}
/* 曲线控件增加一个点 */
#define __sendpoint(__ID__, __CHNL__, __POINT__)                                                    \
	{                                                                                               \
		bsprif1("add %d,%d,%d", hmi_id = (__ID__), hmi_chnl = (__CHNL__), hmi_point = (__POINT__)); \
		__prifend1;                                                                                 \
	}
/* 曲线控件添加多个点(透传) */
#define __sendpoints(__ID__, __CHNL__, __COUNT__)                                                           \
	{                                                                                                       \
		bsprif1("addt %d,%d,%d", hmi_id = (__ID__), hmi_chnl = (__CHNL__), hmi_point_counts = (__COUNT__)); \
		__prifend1;                                                                                         \
	}

/* 单通道特性透传曲线 */
void BSP_HMI_SendCurveSingle(uint8_t id, uint8_t chnl, uint8_t points[], uint16_t len, uint8_t page);

#ifdef __cplusplus
}
#endif
#endif /* __BSP_USART_HMI_H__ */
