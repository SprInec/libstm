/**
 * @file bsp_usart_hmi.c
 * @author July (Email: JulyCub@163.com)
 * @brief The board support package for USART HMI.
 * @version 1.0
 * @date 2022.07.06
 * @update 2023.07.31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "bsp_usart_hmi.h"
#include "string.h"

/* HMI通道 */
uint8_t    hmi_chnl;
/* 控件ID */
uint8_t 	hmi_id;		
/* 页面标号 */
uint8_t 	hmi_page;	
uint16_t 	hmi_point; 
uint16_t 	hmi_point_counts;

/**
 * @brief 单通道特性透传曲线
 * @param id 控件ID
 * @param chnl 通道号
 * @param points 曲线点集
 * @param len 点集长度
 * @param page 页面标号
 */
void BSP_HMI_SendCurveSingle(uint8_t id, uint8_t chnl, uint8_t points[], uint16_t len, uint8_t page)
{
	uint16_t	i = len - 1;

	/* 切换页面 */
	__switchpage(page);
	/* 多点发送设置 */
	__sendpoints(id, chnl, len);
	while(i >= 0){
		bsprif1("\"%d\"", points[i--]);
		if(i < len - 1){
			bsprif1("\",\"");
		}
	}
}
