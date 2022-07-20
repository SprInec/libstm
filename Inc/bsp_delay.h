/*
 ******************************************************************************
 * @file		:bsp_delay.h
 * @brief		:使用SysTick的普通计数模式对延迟进行管理.
 * @version	:1.0
 * @author	:merchant
 * @date		:2022.07.05
 ******************************************************************************
 * @attention
 *
 *
 ******************************************************************************
 */
 
#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__

#include "bsp_config.h"

/**********************************************************
                     外部函数头文件                        
**********************************************************/

#include "bsp_sys.h"

/**********************************************************
                  延时函数计数来源定义
**********************************************************/

#define _USER_SysTick					1	/* 定义是否使用SysTick定时器做延时计数函数 */
																//0：使用其他方式
																//1：使用SysTick定时器做延时函数计数

/**********************************************************
                   定义其他延时计数资源
**********************************************************/

#if !_USER_SysTick	//使用其他方式做延时函数计数

#define DWT_CTRL				*(volatile uint32_t*)0xe0001000	//DWT控制寄存器
#define DWT_CYCCNT				*(volatile uint32_t*)0xe0001004	//DWT当前PC采样周期计数寄存器
																//详细见：周立功《CM3计数参考手册》第129页介绍
#define DWT_CPICNT				*(volatile uint32_t*)0xe0001008	//DWT当前CPI计数寄存器
#define DEM_CTRL				*(volatile uint32_t*)0xe000edfc	//调试异常和监控控制寄存器

#endif

/**********************************************************
                     外部功能函数
**********************************************************/

#if _USER_SysTick	//定义了则使用SysTick定时器做延时函数计数

void delay_init(uint8_t SYSCLK);	//初始化延时函数
void delay_us(uint32_t nus);	//延时n个us
void delay_ms(uint16_t nms);	//延时n个ms
void delay_100ns(void);
void HAL_Delay_us(uint32_t us);

#else	//否则使用其他方式

void delay_init(uint8_t SYSCLK);	//初始化延时函数
void delay_us(uint32_t nus);	//延时n个us

//延时ms级定义，延时范围：1 ~ 65535ms。延时最大值可变，不爆机uint32_t/1000范围即可
#define delay_ms(nms)			delay_us((uint16_t)nms*1000)

#endif	//end _USER_SysTick

#endif /* __CORE_DELAY_H */

